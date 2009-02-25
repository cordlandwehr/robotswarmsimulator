/*
 * stats_control.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include "stats_control.h"

StatsControl::StatsControl() {
	stats_initialized_ = false;
}

StatsControl::~StatsControl() {
	quit();
}

void StatsControl::init(map<std::string, std::string> &params) {

	if (stats_initialized_) {
		// log error, because no quit was called before this init
		std::cerr << "StatsControl::init(...) called without any previous StatsControl::quit(...)" << std::endl;
		// manually quit current statistic-calculation
		quit();
	}

	// initialize StatsConfig
	stats_cfg_.init(params);

	// abort if nothing to calculate
	if (!stats_cfg_.is_any_subset())
		return;

	string dir = "";

	// create a StatsOut-instance for each subset
	if (stats_cfg_.is_subset_all())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ALL", dir)));
	if (stats_cfg_.is_subset_actall())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTALL", dir)));
	if (stats_cfg_.is_subset_inactall())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTALL", dir)));
	if (stats_cfg_.is_subset_masters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("MASTER", dir)));
	if (stats_cfg_.is_subset_actmasters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTMASTER", dir)));
	if (stats_cfg_.is_subset_inactmasters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTMASTER", dir)));
	if (stats_cfg_.is_subset_slaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("SLAVE", dir)));
	if (stats_cfg_.is_subset_actslaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTSLAVE", dir)));
	if (stats_cfg_.is_subset_inactslaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTSLAVE", dir)));

	// create data-praefix for all StatsOut-instances
	// by calling the respective static function
	StatsOut::create_date();

	// initialize stats_calc_indata_
	stats_calc_indata_.prev_world_info_.reset();
	stats_calc_indata_.world_info_.reset();

	// initialize StatsCalc
	stats_calc_.init(&stats_cfg_);
}

void StatsControl::update(const WorldInformation& world_information, boost::shared_ptr<Event> event) {
	if (DEBUG) {
		std::cout << "DEBUG: >>>> StatsControl::update(...) with WorldInformation.time==" <<  world_information.time() << std::endl;
	}

	if (stats_calc_indata_.world_info_.get() == NULL) {
		if (DEBUG)
			std::cout << "DEBUG:      no current world_info, so set it to given" << std::endl;
		// there's no current world_info_ (== in the first simulation-step) so set it and wait
		// for any latter one with the same world-time.
		stats_calc_indata_.world_info_ = boost::shared_ptr<WorldInformation>(new WorldInformation(world_information));

	} else if (stats_calc_indata_.world_info_.get()->time() == world_information.time()) {
		if (DEBUG)
			std::cout << "DEBUG:      existing current world_info for same time, so keep latter one" << std::endl;

		// there's already a world_info_ for the same time, so overwrite it with the current latter one
		// but wait if more updates for the same world-time will follow.
		stats_calc_indata_.world_info_ = boost::shared_ptr<WorldInformation>(new WorldInformation(world_information));

	} else if (stats_calc_indata_.world_info_.get()->time() < world_information.time()) {
		if (DEBUG)
			std::cout << "DEBUG:      existing current world_info for earlier time, so do the calculation on it" << std::endl;

		// the already existing world_info is the latest for the old time.
		// So NOW do all the calculation and data-output to files...
		calculate();

		// Move the world_info_ to prev_world_info_
		// (any existing prev_world_info_-content will be freed through the shared_ptr's destructor)
		stats_calc_indata_.prev_world_info_ = stats_calc_indata_.world_info_;

		// Set the new world_information from this update-call as the new world_info_
		stats_calc_indata_.world_info_ = boost::shared_ptr<WorldInformation>(new WorldInformation(world_information));

	} else {
		std::cerr << "Error in StatsControl::update(...): unhandled case that must not occur." << std::endl;
	}

	if (DEBUG) {
		std::cout << "DEBUG:      now world_info_.time()==" << stats_calc_indata_.world_info_.get()->time() << std::endl;
		if (stats_calc_indata_.prev_world_info_.get() == NULL) {
			std::cout << "DEBUG:      now prev_world_info_==NULL" << std::endl;
		} else {
			std::cout << "DEBUG:      now prev_world_info_.time()=" << stats_calc_indata_.prev_world_info_.get()->time() << std::endl;
		}
		std::cout << "DEBUG: <<<<" << std::endl;
	}

}

void StatsControl::quit() {
	// clear all subsets
	for (unsigned int i=0; i<cur_subsets_.size(); i++)
		cur_subsets_[i].clear();

	// clear the vector of subsets itself
	cur_subsets_.clear();

	// quit all StatsOut
	for (unsigned int i=0; i<stats_out_.size(); i++)
		(*stats_out_[i]).quit();

	// clear the vector of StatsOuts
	stats_out_.clear();

	stats_initialized_ = false;

	std::cout << "Statistics-module quit." << std::endl;
}

void StatsControl::calculate() {
	if (DEBUG)
		std::cout << "DEBUG: >>>>>>>> StatsControl::calculate()" << std::endl;

	if (stats_initialized_) {
		// log error, because not initialized - but continue
		std::cerr << "StatsControl::calculate(...) called without any previous StatsControl::init(...)" << std::endl;
	}

	if (!stats_cfg_.is_any_subset())
		return;

	// the most recent RobotData-instances in the subsets have changed,
	// so recalculate all of them.
	update_subsets();

	if (DEBUG) {
		std::cout << "DEBUG:          number of subsets: " << cur_subsets_.size() << std::endl;
		for(unsigned int i=0; i<cur_subsets_.size(); i++) {
			std::cout << "DEBUG:          size of subset " << i << " is " << cur_subsets_[i].size() << std::endl;
		}
	}

	// for each subset perform the calculation
	// with the respective StatsOut-instance.
	// asserts that stats_calc_indata_ contains valid information
	for(unsigned int i=0; i<cur_subsets_.size(); i++)
		stats_calc_.calculate(stats_calc_indata_, cur_subsets_[i], stats_out_[i]);

	if (DEBUG)
		std::cout << "DEBUG: <<<<<<<<" << std::endl;
}


void StatsControl::update_subsets() {

	// clear all subsets
	for (unsigned int i=0; i<cur_subsets_.size(); i++)
		cur_subsets_[i].clear();

	// clear the vector of subsets itself
	cur_subsets_.clear();

	// build new subsets
	std::vector<boost::shared_ptr<RobotData> > all;
	std::vector<boost::shared_ptr<RobotData> > actall;
	std::vector<boost::shared_ptr<RobotData> > inactall;
	std::vector<boost::shared_ptr<RobotData> > masters;
	std::vector<boost::shared_ptr<RobotData> > actmasters;
	std::vector<boost::shared_ptr<RobotData> > inactmasters;
	std::vector<boost::shared_ptr<RobotData> > slaves;
	std::vector<boost::shared_ptr<RobotData> > actslaves;
	std::vector<boost::shared_ptr<RobotData> > inactslaves;

	std::vector<boost::shared_ptr<RobotData> >& robots = stats_calc_indata_.world_info_->robot_data();

	for (unsigned int i=0; i<robots.size(); i++) {
		boost::shared_ptr<RobotData> sp_robot = robots[i];

		if (stats_cfg_.is_subset_all())
			all.push_back(sp_robot);

		if ( (*sp_robot).type() == MASTER) {
			// type is MASTER
			if (stats_cfg_.is_subset_masters())
				masters.push_back(sp_robot);

			if ((*sp_robot).status() == READY) {
				// status is READY
				if (stats_cfg_.is_subset_actall())
					actall.push_back(sp_robot);

				if (stats_cfg_.is_subset_actmasters())
					actmasters.push_back(sp_robot);

			} else {
				// status is SLEEPING
				if (stats_cfg_.is_subset_inactall())
					inactall.push_back(sp_robot);

				if (stats_cfg_.is_subset_inactmasters())
					inactmasters.push_back(sp_robot);
			}
		} else {
			// type is SLAVE
			if (stats_cfg_.is_subset_slaves())
				slaves.push_back(sp_robot);

			if ((*sp_robot).status() == READY) {
				// status is READY
				if (stats_cfg_.is_subset_actall())
					actall.push_back(sp_robot);

				if (stats_cfg_.is_subset_actslaves())
					actslaves.push_back(sp_robot);
			} else {
				// status is SLEEPING
				if (stats_cfg_.is_subset_inactall())
					inactall.push_back(sp_robot);

				if (stats_cfg_.is_subset_inactslaves())
					inactslaves.push_back(sp_robot);
			}
		}
	}

	// push subsets to cur_subsets
	if (stats_cfg_.is_subset_all())
		cur_subsets_.push_back(all);
	if (stats_cfg_.is_subset_actall())
		cur_subsets_.push_back(actall);
	if (stats_cfg_.is_subset_inactall())
		cur_subsets_.push_back(inactall);

	if (stats_cfg_.is_subset_masters())
		cur_subsets_.push_back(masters);
	if (stats_cfg_.is_subset_actmasters())
		cur_subsets_.push_back(actmasters);
	if (stats_cfg_.is_subset_inactmasters())
		cur_subsets_.push_back(inactmasters);

	if (stats_cfg_.is_subset_slaves())
		cur_subsets_.push_back(slaves);
	if (stats_cfg_.is_subset_actslaves())
		cur_subsets_.push_back(actslaves);
	if (stats_cfg_.is_subset_inactslaves())
		cur_subsets_.push_back(inactslaves);
}

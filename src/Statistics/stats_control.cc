/*
 * stats_control.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include <boost/smart_ptr.hpp>

#include <vector>

#include "../Model/robot_data.h"

#include "stats_control.h"
#include "stats_config.h"

StatsControl::StatsControl() {
	stats_initialized_ = false;
}

StatsControl::~StatsControl() {
	quit();
}

void StatsControl::init(Parser& parser) {

	if (stats_initialized_) {
		// log error, because no quit was called before this init
		std::cerr << "StatsControl::init(...) called without any previous StatsControl::quit(...)" << std::endl;
		// manually quit current statistic-calculation
		quit();
	}

	// initialize StatsConfig
	stats_cfg_.init(parser);

	// abort if nothing to calculate
	if (!stats_cfg_.is_any_subset())
		return;

	if (stats_cfg_.is_subset_all())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ALL")));
	if (stats_cfg_.is_subset_actall())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTALL")));
	if (stats_cfg_.is_subset_inactall())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTALL")));
	if (stats_cfg_.is_subset_masters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("MASTER")));
	if (stats_cfg_.is_subset_actmasters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTMASTER")));
	if (stats_cfg_.is_subset_inactmasters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTMASTER")));
	if (stats_cfg_.is_subset_slaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("SLAVE")));
	if (stats_cfg_.is_subset_actslaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTSLAVE")));
	if (stats_cfg_.is_subset_inactslaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTSLAVE")));

	// TODO
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

	stats_out_.clear();

	// TODO
}

void StatsControl::calculate() {
	if (!stats_cfg_.is_any_subset())
		return;

	// the most recent RobotData-instances in the subsets have changed,
	// so recalculate all of them.
	update_subsets();

	// TODO

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

	std::vector<boost::shared_ptr<RobotData> >& robots = stats_calc_indata_.world_info_.robot_data();

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

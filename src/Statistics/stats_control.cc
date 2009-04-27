#include "stats_control.h"
#include <SimulationControl/time_point.h>
#include <Utilities/console_output.h>
#include "statistics_data_object.h"

StatsControl::StatsControl() {
	stats_initialized_ = false;
}

StatsControl::~StatsControl() {
	if (stats_initialized_) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::warning) << "No explicit quit called - now terminated by deconstructor";
		quit();
	}
}

void StatsControl::init(map<std::string, std::string> &params, std::string output_dir) {

	if (stats_initialized_) {
		// log warning, because no quit was called before this init
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::warning) << "StatsControl::init(...) called without any previous StatsControl::quit(...). Now auto-quitting...";
		// quit current statistic-calculation
		quit();
	}

	// initialize StatsConfig
	stats_cfg_.init(params);

	// abort if nothing to calculate
	if (!stats_cfg_.is_any_subset())
		return;

	//TODO (cola) test what happens if does not exist
	// sets and creates output dir
	if (output_dir.compare("")!=0 && !boost::filesystem::exists(output_dir)) {
		boost::filesystem::create_directory( output_dir );
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Directory "
		                                                                   << output_dir
		                                                                   << " was created.";
	}

	// create a StatsOut-instance for each subset
	if (stats_cfg_.is_subset_all())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ALL", output_dir)));
	if (stats_cfg_.is_subset_actall())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTALL", output_dir)));
	if (stats_cfg_.is_subset_inactall())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTALL", output_dir)));
	if (stats_cfg_.is_subset_masters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("MASTERS", output_dir)));
	if (stats_cfg_.is_subset_actmasters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTMASTERS", output_dir)));
	if (stats_cfg_.is_subset_inactmasters())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTMASTERS", output_dir)));
	if (stats_cfg_.is_subset_slaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("SLAVES", output_dir)));
	if (stats_cfg_.is_subset_actslaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("ACTSLAVES", output_dir)));
	if (stats_cfg_.is_subset_inactslaves())
		stats_out_.push_back(boost::shared_ptr<StatsOut>(new StatsOut("INACTSLAVES", output_dir)));

	// create data-prefix for all StatsOut-instances
	// by calling the respective static function
	StatsOut::create_date();

	// initialize stats_calc_indata_
	stats_calc_indata_.prev_world_info_.reset();
	stats_calc_indata_.world_info_.reset();
	for (unsigned int i=0; i<stats_out_.size(); i++) {
		boost::shared_ptr<std::vector<Vector3d> > foo = boost::shared_ptr<std::vector<Vector3d> >(new std::vector<Vector3d>());
		stats_calc_indata_.prev_positions.push_back(foo);

		boost::shared_ptr<Vector3d> fooVec = boost::shared_ptr<Vector3d>(new Vector3d());
		stats_calc_indata_.prev_miniball_center.push_back(fooVec);

		stats_calc_indata_.prev_miniball_radius.push_back(-1);
	}

	// initialize StatsCalc
	stats_calc_.init(&stats_cfg_);

	// create datadump-instance
	switch(stats_cfg_.datadump_level()) {
		case StatsConfig::DATADUMP_NONE :
			break;

		case StatsConfig::DATADUMP_FULL :
			stats_datadump_ = boost::shared_ptr<StatsOut>(new StatsOut("DATADUMP_FULL", output_dir));
			break;

		default :
			ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::error) << "In Stats_control.init(...): unspecified datadump_level()==" << stats_cfg_.datadump_level();
	}
}

void StatsControl::update(const TimePoint& time_point, boost::shared_ptr<Event> event) {
	const WorldInformation & world_information = time_point.world_information();
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug)  << "StatsControl::update(...) with WorldInformation.time==" <<  world_information.time();


	if (stats_calc_indata_.world_info_.get() == NULL) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "no current world_info, so set it to given";
		// there's no current world_info_ (== in the first simulation-step) so set it and wait
		// for any latter one with the same world-time.
		stats_calc_indata_.world_info_ = boost::shared_ptr<WorldInformation>(new WorldInformation(world_information));

	} else if (stats_calc_indata_.world_info_.get()->time() == world_information.time()) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "existing current world_info for same time, so keep latter one";

		// there's already a world_info_ for the same time, so overwrite it with the current latter one
		// but wait if more updates for the same world-time will follow.
		stats_calc_indata_.world_info_ = boost::shared_ptr<WorldInformation>(new WorldInformation(world_information));

	} else if (stats_calc_indata_.world_info_.get()->time() < world_information.time()) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "existing current world_info for earlier time, so do the calculation on it";

		// the already existing world_info is the latest for the old time.
		// So NOW do all the calculation and data-output to files...
		calculate();

		// Move the world_info_ to prev_world_info_
		// (any existing prev_world_info_-content will be freed through the shared_ptr's destructor)
		stats_calc_indata_.prev_world_info_ = stats_calc_indata_.world_info_;

		// Set the new world_information from this update-call as the new world_info_
		stats_calc_indata_.world_info_ = boost::shared_ptr<WorldInformation>(new WorldInformation(world_information));

	} else {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::error) << "Error in StatsControl::update(...): unhandled case that must not occur.";
	}

	if (stats_cfg_.datadump_level() != StatsConfig::DATADUMP_NONE) {
		do_datadump(world_information, event);
	}

	// some debug output
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "now world_info_.time()==" << stats_calc_indata_.world_info_.get()->time();
	if (stats_calc_indata_.prev_world_info_.get() == NULL) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "now prev_world_info_==NULL";
	} else {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "now prev_world_info_.time()=" << stats_calc_indata_.prev_world_info_.get()->time();
	}

}

void StatsControl::do_datadump(const WorldInformation& world_information, boost::shared_ptr<Event> event) {
	std::vector<std::string> names;
	std::vector<double> values;

	switch(stats_cfg_.datadump_level()) {
		case StatsConfig::DATADUMP_FULL :
			if (!stats_datadump_->is_open()) {
				// prepare the fieldnames
				names.push_back("eventtime");
				names.push_back("num_robots");
				for (unsigned int i=0; i<world_information.robot_data().size(); i++) {
					names.push_back("robot_id");
					names.push_back("robot_type");
					names.push_back("robot_status");
					names.push_back("robot_pos_x");
					names.push_back("robot_pos_y");
					names.push_back("robot_pos_z");
					names.push_back("robot_vel_x");
					names.push_back("robot_vel_y");
					names.push_back("robot_vel_z");
					names.push_back("robot_acc_x");
					names.push_back("robot_acc_y");
					names.push_back("robot_acc_z");
				}

				// open outfile and stream the fieldnames
				stats_datadump_->open(names, false);
			}

			// stream out values
			if (event.get() != NULL)
				values.push_back(event->time());
			else
				values.push_back(-1);

			values.push_back(world_information.robot_data().size());
			for (unsigned int i=0; i<world_information.robot_data().size(); i++) {
				RobotData *rb = world_information.robot_data()[i].get();
				values.push_back(i);
				values.push_back(rb->type());
				values.push_back(rb->status());
				values.push_back(rb->position()[0]);
				values.push_back(rb->position()[1]);
				values.push_back(rb->position()[2]);
				values.push_back(rb->velocity()[0]);
				values.push_back(rb->velocity()[1]);
				values.push_back(rb->velocity()[2]);
				values.push_back(rb->acceleration()[0]);
				values.push_back(rb->acceleration()[1]);
				values.push_back(rb->acceleration()[2]);
			}

			stats_datadump_->update(world_information.time(), values);
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
		stats_out_[i]->quit();

	// clear the vector of StatsOuts
	stats_out_.clear();

	// quit the datadump - if any
	if (stats_datadump_.get() != NULL)
		stats_datadump_->quit();

	stats_initialized_ = false;

	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Output closed.";
}

void StatsControl::calculate() {
	if (DEBUG)
		std::cout << "DEBUG: >>>>>>>> StatsControl::calculate()" << std::endl;

	if (stats_initialized_) {
		// log error, because not initialized - but continue
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::error) << "StatsControl::calculate(...) called without any previous StatsControl::init(...)";
	}

	if (!stats_cfg_.is_any_subset())
		return;

	// the most recent RobotData-instances in the subsets have changed,
	// so recalculate all of them.
	update_subsets();

	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "number of subsets: " << cur_subsets_.size() << std::endl;
	for(unsigned int i=0; i<cur_subsets_.size(); i++) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "size of subset " << i << " is " << cur_subsets_[i].size();
	}


	// for each subset perform the calculation
	// with the respective StatsOut-instance.
	// asserts that stats_calc_indata_ contains valid information
	for(unsigned int i=0; i<cur_subsets_.size(); i++)
		stats_calc_.calculate(stats_calc_indata_, cur_subsets_[i], stats_out_[i], i);
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

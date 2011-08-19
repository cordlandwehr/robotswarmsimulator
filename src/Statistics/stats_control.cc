/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stats_control.h"
#include <SimulationControl/time_point.h>
#include <Utilities/console_output.h>
#include "statistics_data_object.h"
#include "../Views/view.h"
#include "../Model/robot_identifier.h"
#include <boost/graph/strong_components.hpp>
#include <boost/graph/graph_utility.hpp>

StatsControl::StatsControl() {
	stats_initialized_ = false;
}

StatsControl::~StatsControl() {
	if (stats_initialized_) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::warning) << "No explicit quit called - now terminated by deconstructor";
		quit();
	}
}

void StatsControl::init(std::map<std::string, std::string> &params, std::string output_dir) {

	if (stats_initialized_) {
		// log warning, because no quit was called before this init
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::warning) << "StatsControl::init(...) called without any previous StatsControl::quit(...). Now auto-quitting...";
		// quit current statistic-calculation
		quit();
	}

	//TODO asetzer check whether this is still needed
	// sets and creates output dir
	if (output_dir.compare("")!=0 && !boost::filesystem::exists(output_dir)) {
		boost::filesystem::create_directory( output_dir );
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Directory "
		                                                                   << output_dir
		                                                                   << " was created.";
	}

	stats_out_ =boost::shared_ptr<StatsOut>(new StatsOut("ALL", output_dir));


	// create data-prefix for all StatsOut-instances
	// by calling the respective static function
	std::string prefix = params["STATISTICS_FILEID"];
	StatsOut::create_prefixes(prefix);

	// initialize stats_calc_indata_
	stats_calc_indata_.prev_world_info_.reset();
	stats_calc_indata_.world_info_.reset();

	//asetzer dunno what this means
	/*// create datadump-instance
	
		case StatsConfig::DATADUMP_FULL :
			stats_datadump_ = boost::shared_ptr<StatsOut>(new StatsOut("DATADUMP_FULL", output_dir));
			break;

		default :
			ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::error) << "In Stats_control.init(...): unspecified datadump_level()==" << stats_cfg_.datadump_level();
	}*/
}

void StatsControl::update(TimePoint& time_point, boost::shared_ptr<Event> event) {
	const WorldInformation & world_information = time_point.world_information();
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug)  << "StatsControl::update(...) with WorldInformation.time==" <<  world_information.time();

	/*boost::shared_ptr<StatisticsDataObject> data(new StatisticsDataObject());*/
	//fill the statistics data object here, if you like
	/*time_point.set_statistics_data_object(data);*/

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
		//stats_calc_indata_.visib_ = data;

	} else if (stats_calc_indata_.world_info_.get()->time() < world_information.time()) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "existing current world_info for earlier time, so do the calculation on it";

		// the already existing world_info is the latest for the old time.

		// So NOW compute the visibilities therefore (err.. already done)
		// ...and NOW do all the calculation and data-output to files...
		calculate();

		// Move the world_info_ to prev_world_info_
		// (any existing prev_world_info_-content will be freed through the shared_ptr's destructor)
		stats_calc_indata_.prev_world_info_ = stats_calc_indata_.world_info_;
		//stats_calc_indata_.prev_visib_ = stats_calc_indata_.visib_;

		// Set the new world_information from this update-call as the new world_info_
		stats_calc_indata_.world_info_ = boost::shared_ptr<WorldInformation>(new WorldInformation(world_information));
		//stats_calc_indata_.visib_ = data;

	} else {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::error) << "Error in StatsControl::update(...): unhandled case that must not occur.";
	}

	//TODO asetzer check if this is right
	/*if (stats_cfg_.datadump_level() != StatsConfig::DATADUMP_NONE) {
		do_datadump(world_information, event);
	}*/

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

	//switch(stats_cfg_.datadump_level()) {
	//	case StatsConfig::DATADUMP_FULL :
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
	//}
}

void StatsControl::quit() {

	stats_out_->quit();
	//stats_out_ = NULL; //TODO asetzer check how this is right

	// quit the datadump - if any
	if (stats_datadump_.get() != NULL)
		stats_datadump_->quit();

	stats_initialized_ = false;

	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Output closed.\n";
}

void StatsControl::calculate() {
	if (DEBUG)
		std::cout << "DEBUG: >>>>>>>> StatsControl::calculate()" << std::endl;

	if (stats_initialized_) {
		// log error, because not initialized - but continue
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::error) << "StatsControl::calculate(...) called without any previous StatsControl::init(...)";
	}


	//start here with calling the appropriate lua script
	
}


/*void StatsControl::update_subsets() {

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
}*/


/*void StatsControl::calculate_visibility_graph(const WorldInformation& world_info,
                                              boost::shared_ptr<StatisticsDataObject> data){
	/*std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots;
	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;

	// init the graph
	boost::shared_ptr<boost::adjacency_list<> > vis_graph(
			new boost::adjacency_list<>(world_info.robot_data().size()));

	boost::shared_ptr<std::vector<int> > components(new std::vector<int>());
	components->resize(world_info.robot_data().size());

	//go through all the robots and their visible neighbors, create edges in graph
	for (it_robot = world_info.robot_data().begin(); it_robot
	!= world_info.robot_data().end(); ++it_robot) {
		boost::shared_ptr<const View> view = (*it_robot)->view().lock();

		if (view) {
			visible_robots = view->get_visible_robots((*it_robot)->robot());

			BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots){
				boost::add_edge((*it_robot)->id()->id(), cur_id->id(), *vis_graph);
			}
		}
	}

	//calculate connected components
	int number_connected_components = boost::strong_components((*vis_graph),&((*components)[0]));

	if (number_connected_components != 1) {
		//single unconnected robots may not get added to the graph - offset accounts for that
		// TODO(peter): is this a bug (because offset is not used) or a leftover of past-days?
		int offset = world_info.robot_data().size() - boost::num_vertices(*vis_graph);
	}

	//set 0 if graph is connected, 1 if it is not
	size_t vis_graph_is_connected = (number_connected_components==1 ? 0 : 1);

	// fill in data object
	data->set_visibility_graph(vis_graph, vis_graph_is_connected, components);
}*/

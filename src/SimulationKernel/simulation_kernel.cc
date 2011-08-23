/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

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

#include "simulation_kernel.h"

#include <map>
#include <string>
#include <boost/any.hpp>

#include "../ActivationSequenceGenerators/activation_sequence_generator.h"

#include "../EventHandlers/event_handler.h"

#include "robot_control.h"
#include "../Views/abstract_view_factory.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/box_identifier.h"
#include "../Model/sphere_identifier.h"
#include "../Model/marker_identifier.h"

#include "../RobotImplementations/simple_robot.h"

#include "../Utilities/VectorModifiers/vector_trimmer.h"
#include "../Utilities/VectorModifiers/vector_difference_trimmer.h"
#include "../Utilities/VectorModifiers/vector_randomizer.h"

#include "factories.h"
#include <Utilities/console_output.h>

#include <iostream>

using namespace std;

// Radius below which robots are counted as being on the same spot
// for multiplicity detection
double kEpsilonRadius = 0.001;

SimulationKernel::SimulationKernel() {
	/* REMARK: If you want to add some Values to the maps, make sure to use only
	 * upper-case letters! All strings read from the projectfiles are converted to upper-case
	 * by boost::to_upper_copy() so it will simply not work if you use lowercase here.
	 */

	// initialize Stati-map
	  robot_status_map_["READY"] = READY;
	  robot_status_map_["SLEEPING"] = SLEEPING;

	// initialize Stati-map
	  robot_type_map_["SLAVE"] = SLAVE;
	  robot_type_map_["MASTER"] = MASTER;
}

SimulationKernel::~SimulationKernel() {

}

const vector<boost::shared_ptr<Robot> >& SimulationKernel::robots() const {
	return robots_;
}

const boost::shared_ptr<History>& SimulationKernel::history() const {
	return history_;
}


void SimulationKernel::init(const string& project_filename,
		                    boost::shared_ptr<History> history,
		                    std::string output_dir,
		                    bool create_statistics) {

	// set history
	history_ = history;

	// create Parser, load project file and get map
	parser_.reset(new Parser());
	parser_->load_projectfiles(project_filename);
	//std::map<std::string, std::string> &params = parser_->parameter_map();
	boost::program_options::variables_map &params_boost = parser_->parameter_map_boost();
	
	ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::info) << "Generated parameter map";
	// create and add initial world information to history
	boost::shared_ptr<WorldInformation> initial_world_information = setup_initial_world_information(parser_);

	boost::shared_ptr<TimePoint> initial_time_point(new TimePoint());
	initial_time_point->set_world_information(initial_world_information);
	history_->insert(initial_time_point);

	// create Robot Control
	boost::shared_ptr<RobotControl> robot_control = Factory::robot_control_factory(params_boost, history_->capacity(), initial_world_information);

	// create ASG
	asg_ = Factory::asg_factory(params_boost);
	asg_->initialize(*history_, robots_, world_modifiers_);

	// create EventHandler.
	event_handler_ = Factory::event_handler_factory(history_, robot_control);

	// create and initialize statistics module;
	if (create_statistics==true) {
		stats_.reset(new StatsControl());
		stats_->init(output_dir);
	} else {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Output disabled.";

		// workaround for visibility-graph in --dry-mode:
		// create and register always a statistics object,
		// but here we do not initialize it => no output
		//stats_.reset(new StatsControl()); //TODO changed by asetzer on August 19th (should work anyway)
	}

	// register SimulationObservers (ViewObject, ASG, maybe StatisticObject)
	event_handler_->register_listener(asg_);
	if (create_statistics==true)
		event_handler_->register_listener(stats_);

	// send initial worldinformation to statistics
	if (create_statistics==true) {
		boost::shared_ptr<Event> foo = boost::shared_ptr<Event>();
		stats_->update(*initial_time_point, foo);
	}

	//TODO asetzer: camera positions hardcoded as a temporary solution
	camera_position_ = "0,0,0";
	camera_direction_ = "1,0,0";
	camera_type_ ="FOLLOW";
}


void SimulationKernel::step() {
	// create a new time point
	boost::shared_ptr<TimePoint> time_point(new TimePoint());

	// fill time point
	event_handler_->handle_event(asg_->get_next_event(), *time_point);

	// insert time point
	history_->insert(time_point);
}


void SimulationKernel::multistep(int steps) {
	for (int i = 0; i < steps; i++) {
		step();
	}
}


void SimulationKernel::quit() {
	last_breath();
	if (stats_!=NULL)
		stats_->quit();
}

void SimulationKernel::create_robots(boost::shared_ptr<Parser> parser, boost::shared_ptr<WorldInformation> initial_world_information) {

	boost::shared_ptr<RobotData> temp_robot_data;
	boost::shared_ptr<Robot> temp_robot;
	boost::shared_ptr<RobotIdentifier> temp_robot_identifier;
	std::string temp_robot_algorithm;

	boost::shared_ptr<Vector3d> temp_robot_position;

	// counting robots by their position vectors
	vector<Vector3d>::size_type robot_count;
	robot_count = parser->robot_ids().size();

	// running through all robots in robotfile
	for (vector<Vector3d>::size_type i = 0; i < robot_count; i++) {
		//TODO(mmarcus) Identifiers are NOT the identifiers of the project file!
		//need to work over parser...
		temp_robot_identifier.reset(new RobotIdentifier(parser->robot_ids()[i]));
		
		temp_robot_algorithm = parser->robot_algorithms()[i];

		temp_robot = Factory::robot_factory(temp_robot_identifier, temp_robot_algorithm);

		temp_robot_position.reset(new Vector3d(parser->robot_positions()[i]));

		temp_robot_data.reset(new RobotData(temp_robot_identifier, temp_robot_position, *temp_robot));

		robots_.push_back(temp_robot);
		initial_world_information->add_robot_data(temp_robot_data);
	}

}

void SimulationKernel::create_edges(boost::shared_ptr< Parser > parser, boost::shared_ptr< WorldInformation > initial_world_information) {

	int temp_edge_end1;
	int temp_edge_end2;
	int temp_edge_bidirectional;
	
	boost::shared_ptr<RobotIdentifier> temp_r1;
	boost::shared_ptr<RobotIdentifier> temp_r2;
	
	boost::shared_ptr<Edge> temp_edge;



	// running through all robots in robotfile
	for (int i = 0; i < parser->edge_end1().size(); ++i) {
		temp_edge_end1 = parser->edge_end1()[i];
		temp_edge_end2 = parser->edge_end2()[i];
		temp_edge_bidirectional = parser->edge_bidirectional()[i];
		
		temp_r1.reset(new RobotIdentifier(temp_edge_end1));
		temp_r2.reset(new RobotIdentifier(temp_edge_end2));
	  
		if (temp_edge_bidirectional) {
		  temp_edge.reset(new UndirectedEdge(temp_r1, temp_r2));
		} else {
		  temp_edge.reset(new DirectedEdge(temp_r1, temp_r2));
		}
	}

}


void SimulationKernel::create_world_modifiers(boost::shared_ptr<Parser> parser) {
	boost::shared_ptr<WorldModifier> temp_world_modifier;
	std::string temp_world_modifier_name;

	vector<string>& temp_world_modifiers = parser->world_modifiers();

	for (int i = 0; i < temp_world_modifiers.size(); ++i) {
		temp_world_modifier_name = temp_world_modifiers[i];
		temp_world_modifier = Factory::world_modifier_factory(temp_world_modifier_name);



		world_modifiers_.push_back(temp_world_modifier);
	}
}

boost::shared_ptr<WorldInformation> SimulationKernel::setup_initial_world_information(
		boost::shared_ptr<Parser> parser) {

	//create new WorldInformation
	boost::shared_ptr<WorldInformation> initial_world_information(new WorldInformation());

	//create robots and add to initial world information
	create_robots(parser, initial_world_information);

	//create robots and add to initial world information
	create_edges(parser, initial_world_information);

	//create world modifiers and add to initial world information
	create_world_modifiers(parser);

	//set time of initial world information
	initial_world_information->set_time(0);

	return initial_world_information;
}

boost::shared_ptr<RequestHandler> SimulationKernel::setup_request_handler(RequestHandlerType req_type,
																		  unsigned int seed,
																		  double discard_prob,
																		  boost::shared_ptr<History> history,
																		  vector<string> vector_modifiers) {

	boost::shared_ptr<RequestHandler> request_handler;
		switch (req_type) {
			case POSITION_REQUEST_HANDLER:
			case MARKER_REQUEST_HANDLER:
				request_handler.reset(new MarkerRequestHandler(seed, discard_prob, *history));
				break;
		}

		return request_handler;
}

void SimulationKernel::setup_vectormodifier(boost::shared_ptr<VectorRequestHandler> request_handler,
																		 vector<string> vector_modifiers) {

	boost::shared_ptr<VectorModifier> vecmod;

	//TODO(mmarcus) here are a lot of dummy values... need to find solution for parametrized vector modifiers
	BOOST_FOREACH( string s, vector_modifiers) {
		switch (vector_modifier_map_[boost::to_upper_copy(s)]) {
			case VECTOR_TRIMMER:
				vecmod.reset(new VectorTrimmer(42.0));
				request_handler->add_vector_modifier(vecmod);
				break;
			case VECTOR_DIFFERENCE_TRIMMER:
				vecmod.reset(new VectorDifferenceTrimmer(42.0));
				request_handler->add_vector_modifier(vecmod);
				break;
			case VECTOR_RANDOMIZER:
				vecmod.reset(new VectorRandomizer(1, 42.0));
				request_handler->add_vector_modifier(vecmod);
				break;
		}
	}
}

void SimulationKernel::dump_simulation() {
	// create a Parser to save actual robotconfiguration
	std::string dumpnumber = boost::lexical_cast<std::string>(parser_->dumpnumber());
	parser_->set_project_filename(("dump_" + dumpnumber).c_str());
	parser_->set_robot_filename(("dump_" + dumpnumber).c_str());
	parser_->save_projectfiles(("dump_" + dumpnumber).c_str(), history_->get_newest().world_information());
}

bool SimulationKernel::terminate_condition(bool run_until_no_multiplicity) const {
	bool result = false;
	if (run_until_no_multiplicity) {
		const TimePoint& newest = history_->get_newest();
		const WorldInformation& world_info = newest.world_information();
		const std::vector<boost::shared_ptr<RobotData> >& robot_data_vec =
		    world_info.robot_data();

		// This is a very naive approach.
		bool no_multiplicity = true;
		BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data, robot_data_vec) {
			BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data_b, robot_data_vec) {
				if(&robot_data->robot() != &robot_data_b->robot()) {
					double distance = vector3d_distance(robot_data->position(),
							                            robot_data_b->position());
					if (distance < kEpsilonRadius) {
						no_multiplicity = false;
						break;
					}

				}
			}
		}
		result = result || no_multiplicity;
	}

	return result;
}

void SimulationKernel::last_breath() const {
	std::cout << "Last breath start" << std::endl;
	const TimePoint& newest = history_->get_newest();
	const WorldInformation& world_info = newest.world_information();
	const std::vector<boost::shared_ptr<RobotData> >& robot_data_vec =
	    world_info.robot_data();

	double max_dist_x = 0.0;
	double max_dist_y = 0.0;
	double max_dist_z = 0.0;

	// Computing the Regularity metric.
	// This is a very naive approach.
	BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data, robot_data_vec) {
		BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data_b, robot_data_vec) {
			if(&robot_data->robot() != &robot_data_b->robot()) {
				double dist_x = (robot_data->position())(kXCoord) -
				                (robot_data_b->position())(kXCoord) + 1;
				if(dist_x < 0) dist_x *= -1;
				max_dist_x = max(max_dist_x, dist_x);

				double dist_y = (robot_data->position())(kYCoord) -
								(robot_data_b->position())(kYCoord) + 1;
				if(dist_y < 0) dist_y *= -1;
				max_dist_y = max(max_dist_y, dist_y);

				double dist_z = (robot_data->position())(kZCoord) -
				                (robot_data_b->position())(kZCoord) + 1;
				if(dist_z < 0) dist_z *= -1;
				max_dist_z = max(max_dist_z, dist_z);
			}
		}
	}

	// This differs by the regularity metric as defined in
	// "Local lattice construction and the Flow algorithm"
	// but only by a constant factor c which is good enough for us.
	double regularity = max(max_dist_x, max_dist_y);
	double gaps = max_dist_x * max_dist_y - robot_data_vec.size();
	regularity = max(regularity, max_dist_z);
	std::cout << std::endl << "REGULARITY: " << regularity << std::endl;
	std::cout << std::endl << "GAPS: " << gaps << std::endl;
}




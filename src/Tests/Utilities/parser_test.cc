/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>

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

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/program_options.hpp>
#include <iostream>

#include "../../Model/sphere.h"
#include "../../Model/sphere_identifier.h"
#include "../../Model/box.h"
#include "../../Model/box_identifier.h"
#include "../../Model/marker_information.h"
#include "../../Model/marker_identifier.h"

#include "../../EventHandlers/event_handler.h"

#include "../ActivationSequenceGenerators/activation_sequence_generator.h"
#include "../ActivationSequenceGenerators/synchronous_asg.h"
#include "../ActivationSequenceGenerators/synchronous_asg_wm.h"

#include "../../ViewModels/global_view.h"
#include "../../ViewModels/local_graph_view.h"
#include "../../Views/view_factory.h"
#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../Utilities/parser.h"
#include "../../Utilities/VectorModifiers/vector_modifier.h"
#include "../../Utilities/VectorModifiers/vector_difference_trimmer.h"
#include "../../Utilities/VectorModifiers/vector_trimmer.h"
#include "../../Utilities/VectorModifiers/vector_randomizer.h"

#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationKernel/factories.h"
#include "../../SimulationControl/history.h"

#include "../Fixtures/simple_world_fixture.h"


BOOST_FIXTURE_TEST_CASE(LoadMainProjectFileTest, SimpleWorldFixture)
{
	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());
	parser->load_projectfiles("../../../src/Tests/TestData/testfile_1");
 	
	//check variables read from main project file
	BOOST_CHECK_EQUAL(parser->parameter_map_boost_["PROJECT_NAME"].as<std::string>(), "My Exciting Project");
 	BOOST_CHECK_EQUAL(parser->robot_filename_, "testfile_1_robots.csv");
 	BOOST_CHECK_EQUAL(parser->edge_filename_, "testfile_1_edges.csv");

	//########################################################################
	//				check request handler stuff
	//########################################################################

 	// RobotControl needed for EventHandler
  	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<LocalGraphView>());
  	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
 

 	boost::program_options::variables_map &params = parser->parameter_map_boost();
 	boost::shared_ptr<EventHandler> event_handler = Factory::event_handler_factory(history, robot_control);
 
 	// marker request handler
 	BOOST_REQUIRE(event_handler);
 	BOOST_REQUIRE(event_handler->marker_request_handler_);
	BOOST_CHECK_EQUAL(0.0, event_handler->marker_request_handler_->discard_probability_);


	//########################################################################
	//				check asg stuff
	//########################################################################

	boost::shared_ptr<ActivationSequenceGenerator> asg = Factory::asg_factory(params);
	BOOST_REQUIRE(boost::dynamic_pointer_cast<SynchronousASGWM>(asg));

}


BOOST_AUTO_TEST_CASE(load_robot_file_1)
{
	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());
	parser->load_projectfiles("../../../src/Tests/TestData/testfile_2");

	//////////////////////////////////////////////////////////
	// Check data of robot 1
	//////////////////////////////////////////////////////////

	//position
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[0](0), 5.3);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[0](1), 9.2);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[0](2), 6.4);

	//algorithm
	BOOST_CHECK_EQUAL(parser->initiale_robot_algorithms_[0], "simple_robot");


	//////////////////////////////////////////////////////////
	// Check data of robot 2
	//////////////////////////////////////////////////////////

	//position
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[1](0), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[1](1), 4.2);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[1](2), 8.8);

	//algorithm
	BOOST_CHECK_EQUAL(parser->initiale_robot_algorithms_[1], "simple_robot");

}


BOOST_AUTO_TEST_CASE(load_edge_file_1)
{
	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());
	parser->load_projectfiles("../../../src/Tests/TestData/testfile_2");

	// Check data of edge 1
	BOOST_CHECK_EQUAL(parser->initiale_edge_end1_[0], 0);
	BOOST_CHECK_EQUAL(parser->initiale_edge_end2_[0], 1);
	BOOST_CHECK_EQUAL(parser->initiale_edge_bidirectional_[0], 0);

	// Check data of edge 2
	BOOST_CHECK_EQUAL(parser->initiale_edge_end1_[1], 1);
	BOOST_CHECK_EQUAL(parser->initiale_edge_end2_[1], 0);
	BOOST_CHECK_EQUAL(parser->initiale_edge_bidirectional_[1], 0);

}


/*
BOOST_FIXTURE_TEST_CASE(save_main_project_file_1, SimpleWorldFixture)
{
	string project_filename = "src/Tests/TestData/garbled_projectfile_c.swarm";

	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());

	//dummy values
	parser->asg_ = "0";
	parser->compass_model_ = "NO_COMPASS";
	parser->obstacle_filename_ = "src/Tests/TestData/obsti.obstacle";
	parser->robot_filename_ = "src/Tests/TestData/i.robot";
	parser->statistics_template_ = "BASIC";
	parser->statistics_subsets_ = "{ALL}";

	parser->save_projectfiles("src/Tests/TestData/garbled_projectfile_c.swarm", history->get_newest());
}

BOOST_FIXTURE_TEST_CASE(write_robot_1, SimpleWorldFixture)
{
	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());

	cout << "<robotwriting-test-case>" << endl;
	cout << parser->write_robot(robot_data_a);
	cout << parser->write_robot(robot_data_b);
	cout << parser->write_robot(robot_data_c);
	cout << "</robotwriting-test-case>" << endl;

}
*/

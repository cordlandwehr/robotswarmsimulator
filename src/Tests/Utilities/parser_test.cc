#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>

#include "../../Model/sphere.h"
#include "../../Model/sphere_identifier.h"
#include "../../Model/box.h"
#include "../../Model/box_identifier.h"
#include "../../Model/marker_information.h"
#include "../../Model/marker_identifier.h"

#include "../../Utilities/parser.h"
#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationControl/history.h"

#include "../Fixtures/simple_world_fixture.h"

BOOST_AUTO_TEST_CASE(load_main_project_file)
{
	cout << "###################### load main project file test - begin ######################" << endl;

	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());
	parser->init();
	parser->load_projectfiles("../RobotSwarmSimulator/src/Tests/TestData/testfile_1");

	//check variables read from main project file
	BOOST_CHECK_EQUAL(parser->project_name_, "My Exciting Project");
	BOOST_CHECK_EQUAL(parser->compass_model_, "NO_COMPASS");
	BOOST_CHECK_EQUAL(parser->robot_filename_, "../RobotSwarmSimulator/src/Tests/TestData/testfile_1");
	BOOST_CHECK_EQUAL(parser->obstacle_filename_, "../RobotSwarmSimulator/src/Tests/TestData/testfile_1");
	BOOST_CHECK_EQUAL(parser->statistics_template_, "BASIC");
	BOOST_CHECK_EQUAL(parser->statistics_subsets_, "{ALL}");
	BOOST_CHECK_EQUAL(parser->asg_, "ASYNCHRONOUS");

	//########################################################################
	//				check request handler stuff
	//########################################################################
	boost::tuple<
		string,						//type of request handler
		double,						//discard probability
		unsigned int,				//seed
		std::vector<boost::tuple<	//list of vector modifiers
			string,						//type of vector modifier
			std::vector<boost::any>		//parameters of vector modifiers
		> > > position_request_handler, velocity_request_handler, acceleration_request_handler;

	boost::tuple<
		string,						//type of request handler
		double,						//discard probability
		unsigned int				//seed
		> marker_request_handler, type_change_request_handler;

	std::vector<boost::tuple<string,std::vector<boost::any> > > vector_modifier_list;
	std::vector<boost::any>	vector_modifier_parameter;

	position_request_handler = parser->position_request_handler_;
	velocity_request_handler = parser->velocity_request_handler_;
	acceleration_request_handler = parser->acceleration_request_handler_;
	marker_request_handler = parser->marker_request_handler_;
	type_change_request_handler = parser->type_change_request_handler_;

	//acceleration request handler
	BOOST_CHECK_EQUAL(boost::get<0>(acceleration_request_handler), "VECTOR");			//type of request handler
	BOOST_CHECK_EQUAL(boost::get<1>(acceleration_request_handler), 0.2);				//discard probability
	BOOST_CHECK_EQUAL(boost::get<2>(acceleration_request_handler), 5);					//seed

	vector_modifier_list = boost::get<3>(acceleration_request_handler);				//vector modifier

	BOOST_CHECK_EQUAL(boost::get<0>(vector_modifier_list[0]), "VECTOR_DIFFERENCE_TRIMMER");	//type of vector modifier 0
	vector_modifier_parameter = boost::get<1>(vector_modifier_list[0]);					//vector modifier parameter
	BOOST_CHECK_EQUAL(boost::any_cast<double>(vector_modifier_parameter[0]), 2.4);		//Parameter 0 of vector modifier 0

	//position request handler
	BOOST_CHECK_EQUAL(boost::get<0>(position_request_handler), "VECTOR");			//type of request handler
	BOOST_CHECK_EQUAL(boost::get<1>(position_request_handler), 0.5);				//discard probability
	BOOST_CHECK_EQUAL(boost::get<2>(position_request_handler), 3);					//seed

	vector_modifier_list = boost::get<3>(position_request_handler);					//vector modifier

	BOOST_CHECK_EQUAL(boost::get<0>(vector_modifier_list[0]), "VECTOR_TRIMMER");		//type of vector modifier 0
	vector_modifier_parameter = boost::get<1>(vector_modifier_list[0]);					//vector modifier parameter
	BOOST_CHECK_EQUAL(boost::any_cast<double>(vector_modifier_parameter[0]), 1.5);		//Parameter 0 of vector modifier 0

	BOOST_CHECK_EQUAL(boost::get<0>(vector_modifier_list[1]), "VECTOR_RANDOMIZER");		//type of vector modifier 1
	vector_modifier_parameter = boost::get<1>(vector_modifier_list[1]);					//vector modifier parameter
	BOOST_CHECK_EQUAL(boost::any_cast<unsigned int>(vector_modifier_parameter[0]), 5);	//Parameter 0 of vector modifier 1
	BOOST_CHECK_EQUAL(boost::any_cast<double>(vector_modifier_parameter[1]), 2.5);		//Parameter 1 of vector modifier 1

	//velocity request handler
	BOOST_CHECK_EQUAL(boost::get<0>(type_change_request_handler), "NONE");				//type of request handler
	BOOST_CHECK_EQUAL(boost::get<1>(type_change_request_handler), 0.1);					//discard probability
	BOOST_CHECK_EQUAL(boost::get<2>(type_change_request_handler), 2);					//seed

	//marker request handler
	BOOST_CHECK_EQUAL(boost::get<0>(marker_request_handler), "STANDARD");			//type of request handler
	BOOST_CHECK_EQUAL(boost::get<1>(marker_request_handler), 0.5);					//discard probability
	BOOST_CHECK_EQUAL(boost::get<2>(marker_request_handler), 1);					//seed

	cout << "###################### load main project file test - end ######################" << endl;

}
/*
BOOST_AUTO_TEST_CASE(load_robot_file_1)
{
	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());
	parser->init();
	parser->load_projectfiles("src/Tests/TestData/garbled_projectfile_a");

	//////////////////////////////////////////////////////////
	// Check data of robot 1
	//////////////////////////////////////////////////////////

	//position
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[0](0), 5.3);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[0](1), 9.2);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[0](2), 6.4);

	//type
	BOOST_CHECK_EQUAL(parser->initiale_robot_types_[0], "master");

	//velocity
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[0](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[0](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[0](2), 3.5);

	//acceleration
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[0](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[0](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[0](2), 3.5);

	//status
	BOOST_CHECK_EQUAL(parser->initiale_robot_stati_[0], "sleeping");

	//status
	BOOST_CHECK_EQUAL(parser->initiale_robot_marker_information_[0], "0");

	//algorithm
	BOOST_CHECK_EQUAL(parser->initiale_robot_algorithms_[0], "MASTER_ALGO");

	//color
	BOOST_CHECK_EQUAL(parser->initiale_robot_colors_[0], "0");

	//x-axis
	BOOST_CHECK_EQUAL(boost::get<0>(parser->initiale_robot_coordinate_sytems_[0])(0), 1);
	BOOST_CHECK_EQUAL(boost::get<0>(parser->initiale_robot_coordinate_sytems_[0])(1), 0);
	BOOST_CHECK_EQUAL(boost::get<0>(parser->initiale_robot_coordinate_sytems_[0])(2), 0);

	//y-axis
	BOOST_CHECK_EQUAL(boost::get<1>(parser->initiale_robot_coordinate_sytems_[0])(0), 0);
	BOOST_CHECK_EQUAL(boost::get<1>(parser->initiale_robot_coordinate_sytems_[0])(1), 1);
	BOOST_CHECK_EQUAL(boost::get<1>(parser->initiale_robot_coordinate_sytems_[0])(2), 0);

	//z-axis
	BOOST_CHECK_EQUAL(boost::get<2>(parser->initiale_robot_coordinate_sytems_[0])(0), 0);
	BOOST_CHECK_EQUAL(boost::get<2>(parser->initiale_robot_coordinate_sytems_[0])(1), 0);
	BOOST_CHECK_EQUAL(boost::get<2>(parser->initiale_robot_coordinate_sytems_[0])(2), 1);


	//////////////////////////////////////////////////////////
	// Check data of robot 2
	//////////////////////////////////////////////////////////

	//position
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[1](0), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[1](1), 4.2);
	BOOST_CHECK_EQUAL(parser->initiale_robot_positions_[1](2), 8.8);

	//type
	//BOOST_CHECK_EQUAL(parser->initiale_robot_types_[0], "slave");

	//velocity
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[1](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[1](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[1](2), 3.5);

	//acceleration
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[1](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[1](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[1](2), 3.5);

	//status
	BOOST_CHECK_EQUAL(parser->initiale_robot_stati_[1], "ready");

	//status
	BOOST_CHECK_EQUAL(parser->initiale_robot_marker_information_[1], "0");

	//algorithm
	BOOST_CHECK_EQUAL(parser->initiale_robot_algorithms_[1], "SLAVE_ALGO");

	//color
	BOOST_CHECK_EQUAL(parser->initiale_robot_colors_[1], "0");

	//x-axis
	BOOST_CHECK_EQUAL(boost::get<0>(parser->initiale_robot_coordinate_sytems_[1])(0), 1);
	BOOST_CHECK_EQUAL(boost::get<0>(parser->initiale_robot_coordinate_sytems_[1])(1), 0);
	BOOST_CHECK_EQUAL(boost::get<0>(parser->initiale_robot_coordinate_sytems_[1])(2), 0);

	//y-axis
	BOOST_CHECK_EQUAL(boost::get<1>(parser->initiale_robot_coordinate_sytems_[1])(0), 0);
	BOOST_CHECK_EQUAL(boost::get<1>(parser->initiale_robot_coordinate_sytems_[1])(1), 1);
	BOOST_CHECK_EQUAL(boost::get<1>(parser->initiale_robot_coordinate_sytems_[1])(2), 0);

	//z-axis
	BOOST_CHECK_EQUAL(boost::get<2>(parser->initiale_robot_coordinate_sytems_[1])(0), 0);
	BOOST_CHECK_EQUAL(boost::get<2>(parser->initiale_robot_coordinate_sytems_[1])(1), 0);
	BOOST_CHECK_EQUAL(boost::get<2>(parser->initiale_robot_coordinate_sytems_[1])(2), 1);

}

BOOST_AUTO_TEST_CASE(load_obstacle_file_1)
{
	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());
	parser->init();
	parser->load_projectfiles("src/Tests/TestData/garbled_projectfile_a");

	// Check data of obstacle 1
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_types_[0], "box");
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_marker_information_[0], "0");
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[0](0), 2.0);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[0](1), 3.0);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[0](2), 4.0);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_size_[0](0), 1.0);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_size_[0](1), 2.0);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_size_[0](2), 3.0);

	// Check data of obstacle 2
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_types_[1], "sphere");
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_marker_information_[1], "0");
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[1](0), 3.4);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[1](1), 5.2);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[1](2), 5.1);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_radius_[1], 5.0);

	// Check data of obstacle 3
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_types_[2], "marker");
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_marker_information_[2], "0");
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[2](0), 3.5);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[2](1), 1.4);
	BOOST_CHECK_EQUAL(parser->initiale_obstacle_positions_[2](2), 5.1);
}
*/

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

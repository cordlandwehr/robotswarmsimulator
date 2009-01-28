#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Utilities/parser.h"
#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationControl/history.h"


BOOST_AUTO_TEST_CASE(load_main_project_file)
{
	Parser* parser = new Parser();
	parser->init();
	parser->load_projectfiles("src/Tests/TestData/garbled_projectfile_a");

	//check variables read from main project file
	BOOST_CHECK_EQUAL(parser->project_name_, "My Exciting Project");
	BOOST_CHECK_EQUAL(parser->compass_model_, "FULL_COMPASS");
	BOOST_CHECK_EQUAL(parser->robot_filename_, "src/Tests/TestData/garbled_projectfile_a");
	BOOST_CHECK_EQUAL(parser->obstacle_filename_, "src/Tests/TestData/garbled_projectfile_a");
	BOOST_CHECK_EQUAL(parser->statistics_module_, "NONE");
	BOOST_CHECK_EQUAL(parser->asg_, 0);
	BOOST_CHECK_EQUAL(parser->event_handler_, 0);
}

BOOST_AUTO_TEST_CASE(load_robot_file_1)
{
	Parser* parser = new Parser();
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
	Parser* parser = new Parser();
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

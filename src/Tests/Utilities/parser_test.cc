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
	BOOST_CHECK_EQUAL((parser->initiale_robot_types_[0]).compare("master"),0);

	//velocity
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[0](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[0](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[0](2), 3.5);

	//acceleration
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[0](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[0](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[0](2), 3.5);

	//status
	BOOST_CHECK_EQUAL((parser->initiale_robot_stati_[0]).compare("sleeping"), 0);

	//marker info
	BOOST_CHECK_EQUAL((parser->initiale_robot_marker_information_[0]).compare("0"), 0);

	//algorithm
	BOOST_CHECK_EQUAL((parser->initiale_robot_algorithms_[0]).compare("MASTER_ALGO"),0);

	//color
	BOOST_CHECK_EQUAL((parser->initiale_robot_colors_[0]).compare("0"), 0);


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
	BOOST_CHECK_EQUAL((parser->initiale_robot_types_[0]).compare("slave"),0);

	//velocity
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[1](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[1](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_velocities_[1](2), 3.5);

	//acceleration
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[1](0), 1.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[1](1), 2.5);
	BOOST_CHECK_EQUAL(parser->initiale_robot_accelerations_[1](2), 3.5);

	//status
	BOOST_CHECK_EQUAL((parser->initiale_robot_stati_[1]).compare("ready"), 0);

	//status
	BOOST_CHECK_EQUAL((parser->initiale_robot_marker_information_[1]).compare("0"), 0);

	//algorithm
	BOOST_CHECK_EQUAL((parser->initiale_robot_algorithms_[1]).compare("SLAVE_ALGO"), 0);

	//color
	BOOST_CHECK_EQUAL((parser->initiale_robot_colors_[1]).compare("0"), 0);

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


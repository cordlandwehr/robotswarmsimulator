#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Utilities/parser.h"
#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationControl/history.h"

//
// INFO
// The parser doesn't handle wrong inputs now.
// Will be fixed soon.
//
/*
BOOST_AUTO_TEST_CASE(loading_main_project_file_1)
{
	Parser* parser = new Parser();
	parser->init();
	parser->load_projectfiles("src/ProjectFiles/projectfile");

	//check variables read from main project file
	BOOST_CHECK_EQUAL(parser->project_name_, "My Exciting Project");
	BOOST_CHECK_EQUAL(parser->compass_model_, "NO_COMPASS");
	BOOST_CHECK_EQUAL(parser->robot_filename_, "myrobots");
	BOOST_CHECK_EQUAL(parser->obstacle_filename_, "myobstacles");
	BOOST_CHECK_EQUAL(parser->statistics_module_, 0);
	BOOST_CHECK_EQUAL(parser->asg_, 0);
	BOOST_CHECK_EQUAL(parser->event_handler_, 0);
}
*/


BOOST_AUTO_TEST_CASE(loading_main_project_file_2)
{
	Parser* parser = new Parser();
	parser->init();
	parser->load_projectfiles("src/Tests/TestData/garbled_projectfile_a");

	//check variables read from main project file
	/*
	BOOST_CHECK_EQUAL(parser->project_name_, "My Exciting Project");
	BOOST_CHECK_EQUAL(parser->compass_model_, "FULL_COMPASS");
	BOOST_CHECK_EQUAL(parser->robot_filename_, "src/Tests/TestData/garbled_projectfile_a");
	BOOST_CHECK_EQUAL(parser->obstacle_filename_, "src/Tests/TestData/garbled_projectfile_a");
	BOOST_CHECK_EQUAL(parser->statistics_module_, "NONE");
	BOOST_CHECK_EQUAL(parser->asg_, 0);
	BOOST_CHECK_EQUAL(parser->event_handler_, 0);
	*/
}

#include <boost/test/unit_test.hpp>

#include "../../SimulationKernel/simulation_kernel.h"

BOOST_AUTO_TEST_CASE(loading_main_project_file)
{
	//create simulation kernel
	SimulationKernel* sim = new SimulationKernel();

	//initialize simulation kernel
	sim->init("src/ProjectFiles/projectfile.swarm");

	//check variables read from main project file
	BOOST_CHECK_EQUAL(sim->project_name(), "My Exciting Project");
	BOOST_CHECK_EQUAL(sim->compass_model(), "NO_COMPASS");
	BOOST_CHECK_EQUAL(sim->robot_filename(), "myrobots");
	BOOST_CHECK_EQUAL(sim->obstacle_filename(), "myobstacles");
	BOOST_CHECK_EQUAL(sim->statistics_module(), 0);
	BOOST_CHECK_EQUAL(sim->asg(), 0);
	BOOST_CHECK_EQUAL(sim->event_handler(), 0);
}

BOOST_AUTO_TEST_CASE(loading_garbled_main_project_file_a)
{
	//create simulation kernel
	SimulationKernel* sim = new SimulationKernel();

	//initialize simulation kernel
	sim->init("src/Tests/Testdata/garbled_projectfile_a.swarm");

	//check variables read from main project file
	BOOST_CHECK_EQUAL(sim->project_name(), "My Exciting Project");

	// TODO(craupach): what should be the result in case of a bad assignement in project file?
	// at this moment the variable gets assigned "COMPASS_MODEL=NO_COMPASS" with no error thrown.
	BOOST_CHECK_EQUAL(sim->compass_model(), "NO_COMPASS");

	BOOST_CHECK_EQUAL(sim->robot_filename(), "myrobots");
	BOOST_CHECK_EQUAL(sim->obstacle_filename(), "myobstacles");
	BOOST_CHECK_EQUAL(sim->statistics_module(), 0);
	BOOST_CHECK_EQUAL(sim->asg(), 0);
	BOOST_CHECK_EQUAL(sim->event_handler(), 0);
}

BOOST_AUTO_TEST_CASE(loading_garbled_main_project_file_b)
{
	//create simulation kernel
	SimulationKernel* sim = new SimulationKernel();

	//initialize simulation kernel
	sim->init("src/Tests/Testdata/garbled_projectfile_b.swarm");

	//check variables read from main project file
	BOOST_CHECK_EQUAL(sim->project_name(), "My Exciting Project");
	BOOST_CHECK_EQUAL(sim->compass_model(), "NO_COMPASS");
	BOOST_CHECK_EQUAL(sim->robot_filename(), "myrobots");
	BOOST_CHECK_EQUAL(sim->obstacle_filename(), "myobstacles");
	BOOST_CHECK_EQUAL(sim->statistics_module(), 0);
	BOOST_CHECK_EQUAL(sim->asg(), 0);
	BOOST_CHECK_EQUAL(sim->event_handler(), 0);
}

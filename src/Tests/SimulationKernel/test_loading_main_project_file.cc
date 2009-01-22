#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationControl/history.h"

BOOST_AUTO_TEST_CASE(loading_main_project_file)
{
	// create empty history to pass along
	// TODO(craupach) should make an extra class for the parser so we can test it seperatly from kernel
	boost::shared_ptr<History> history(new History(10));

	//create simulation kernel
	SimulationKernel* sim = new SimulationKernel();

	//initialize simulation kernel
	sim->init("src/ProjectFiles/projectfile.swarm", history);

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
	// create empty history to pass along
	// TODO(craupach) should make an extra class for the parser so we can test it seperatly from kernel
	boost::shared_ptr<History> history(new History(10));

	//create simulation kernel
	SimulationKernel* sim = new SimulationKernel();

	//initialize simulation kernel
	sim->init("src/Tests/Testdata/garbled_projectfile_a.swarm", history);

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
	// create empty history to pass along
	// TODO(craupach) should make an extra class for the parser so we can test it seperatly from kernel
	boost::shared_ptr<History> history(new History(10));

	//create simulation kernel
	SimulationKernel* sim = new SimulationKernel();

	//initialize simulation kernel
	sim->init("src/Tests/Testdata/garbled_projectfile_b.swarm", history);

	//check variables read from main project file
	BOOST_CHECK_EQUAL(sim->project_name(), "My Exciting Project");
	BOOST_CHECK_EQUAL(sim->compass_model(), "NO_COMPASS");
	BOOST_CHECK_EQUAL(sim->robot_filename(), "myrobots");
	BOOST_CHECK_EQUAL(sim->obstacle_filename(), "myobstacles");
	BOOST_CHECK_EQUAL(sim->statistics_module(), 0);
	BOOST_CHECK_EQUAL(sim->asg(), 0);
	BOOST_CHECK_EQUAL(sim->event_handler(), 0);
}

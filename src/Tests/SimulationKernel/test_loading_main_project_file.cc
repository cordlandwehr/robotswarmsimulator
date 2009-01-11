#include <boost/test/unit_test.hpp>

#include "../../SimulationKernel/simulation_kernel.h"

BOOST_AUTO_TEST_CASE(loading_main_project_file)
{
	//create simulation kernel
	SimulationKernel* sim = new SimulationKernel();
	sim->init("/home/martinah/Studium/PG/SVN/der-schwarm/codebase/RobotSwarmSimulator/src/ProjectFiles/projectfile.swarm");

	BOOST_CHECK_EQUAL(sim->project_name(), "My Exciting Project");
	BOOST_CHECK_EQUAL(sim->compass_model(), "NO_COMPASS");
	BOOST_CHECK_EQUAL(sim->robot_filename(), "myrobots");
	BOOST_CHECK_EQUAL(sim->obstacle_filename(), "myobstacles");
	BOOST_CHECK_EQUAL(sim->statistics_module(), 0);
	BOOST_CHECK_EQUAL(sim->asg(), 0);
	BOOST_CHECK_EQUAL(sim->event_handler(), 0);
	BOOST_CHECK_EQUAL(sim->algorithm(),"CLEVER_ALGO");

}

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include "../../Events/compute_event.h"
#include "../../Model/robot.h"

/*
 * Helper function to test adding a robot in a different scope
 */
void add_robot(ComputeEvent& event) {
    boost::shared_ptr<Robot> robot;
    robot.reset(new Robot());
    event.add_to_robot_subset(robot);
}

BOOST_AUTO_TEST_CASE(compute_event_smoke_test)
{
    ComputeEvent event(5);

    // time correct
    BOOST_CHECK_EQUAL(event.get_time(), 5);

    // no robots in the subset at beginning
    BOOST_CHECK_EQUAL(event.get_robot_subset().size(), 0);

    // create a shared pointer on a robot
    boost::shared_ptr<Robot> robot;
    robot.reset(new Robot());
    event.add_to_robot_subset(robot);

    // there should be a robot in the subset now
    BOOST_CHECK_EQUAL(event.get_robot_subset().size(), 1);

	// add a robot in a different scope
    add_robot(event);

    // there shoud be two robots in the subset now
    BOOST_CHECK_EQUAL(event.get_robot_subset().size(), 2);
}

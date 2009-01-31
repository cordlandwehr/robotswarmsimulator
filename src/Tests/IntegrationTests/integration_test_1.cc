/*
 * integration_test_1.cc
 *
 *  Created on: 27.01.2009
 *      Author: craupach
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../ActivationSequenceGenerators/synchronous_asg.h"
#include "../../EventHandlers/event_handler.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/robot_control.h"
#include "../../Views/abstract_view_factory.h"
#include "../../Views/global_view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/integration_fixture.h"
#include <iostream>

/*
 * Test: See if EventHandler, ASG, View and a simple cog algorithm work together well.
 * Expected Results: All robots are at cog after three events
 *
 */
BOOST_FIXTURE_TEST_CASE(integration_test_1, IntegrationFixture) {
	// setup of view
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<GlobalView>());
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, kHistorySize, *initial_world_information));

	// setup of activation sequence generator
	boost::shared_ptr<SynchronousASG> asg(new SynchronousASG());
	asg->initialize(*history, robots);

	// setup of event handler and register the robot control and asg as listeners
	EventHandler event_handler(history, robot_control);
	boost::shared_ptr<SimulationListener> robot_control_listener =
	    boost::dynamic_pointer_cast<SimulationListener>(robot_control);
	boost::shared_ptr<SimulationListener> asg_listener =
		    boost::dynamic_pointer_cast<SimulationListener>(asg);
	event_handler.register_listener(asg_listener);


	// simulate a few steps (since this is synchronous time this is a look-compute-move cycle)
	for(int i = 0; i < 3; i++) {
		// get next event
		boost::shared_ptr<Event> event = asg->get_next_event();
		// handle next event
		event_handler.handle_event(event);
		// consume event
	    history->get_oldest_unused(); // prevent blocking
	}

	// all robots should be at the cog now
	WorldInformation newest = history->get_newest();
	BOOST_CHECK_EQUAL(newest.time(), 2); // events at t = 0,1,2 have happened.

	// all robots should be at the same point.
	BOOST_CHECK_CLOSE((*newest.robot_data().at(0)->extrapolated_position(0.0))(kXCoord),
	                  (*newest.robot_data().at(1)->extrapolated_position(0.0))(kXCoord),
	                  0.1);
	BOOST_CHECK_CLOSE((*newest.robot_data().at(0)->extrapolated_position(0.0))(kXCoord),
	                  (*newest.robot_data().at(2)->extrapolated_position(0.0))(kXCoord),
	                  0.1);
	BOOST_CHECK_CLOSE((*newest.robot_data().at(0)->extrapolated_position(0.0))(kYCoord),
	                  (*newest.robot_data().at(1)->extrapolated_position(0.0))(kYCoord),
	                  0.1);
	BOOST_CHECK_CLOSE((*newest.robot_data().at(0)->extrapolated_position(0.0))(kYCoord),
	                  (*newest.robot_data().at(2)->extrapolated_position(0.0))(kYCoord),
	                  0.1);
	BOOST_CHECK_CLOSE((*newest.robot_data().at(0)->extrapolated_position(0.0))(kZCoord),
	                  (*newest.robot_data().at(1)->extrapolated_position(0.0))(kZCoord),
	                  0.1);
	BOOST_CHECK_CLOSE((*newest.robot_data().at(0)->extrapolated_position(0.0))(kZCoord),
	                  (*newest.robot_data().at(2)->extrapolated_position(0.0))(kZCoord),
	                  0.1);

}

/*
 * fair_atomic_semisynchronous_asg_test.cc
 *
 *  Created on: Aug 3, 2009
 *      Author: dwonisch
 */

#include "../../ActivationSequenceGenerators/fair_atomic_semisynchronous_asg.h"

#include "../../Model/world_information.h"
#include "../../Events/look_event.h"
#include "../../Events/compute_event.h"
#include "../../Events/handle_requests_event.h"
#include "../../Events/event.h"
#include "../../Model/robot.h"
#include "../../SimulationControl/history.h"
#include "../Fixtures/simple_world_fixture.h"

#include <iostream>
#include <set>

BOOST_FIXTURE_TEST_CASE(fair_atomic_semisynchronous_asg_smoke_test, SimpleWorldFixture)
{
	FairAtomicSemisynchronousASG asg(517);
	asg.initialize(*history,robots);

	// first event should be at time 0
	BOOST_CHECK_EQUAL(asg.get_time_of_next_event(), 0);

	for(unsigned k = 0; k < 5; k++) {
		std::set<boost::shared_ptr<Robot> > activated_robots;
		for(unsigned i = 0; i < robots.size(); i++) {
			boost::shared_ptr<Robot> active_robot;
			
			boost::shared_ptr<Event> next_event = asg.get_next_event();
			LookEvent * look_event = dynamic_cast<LookEvent*>(next_event.get());
			BOOST_REQUIRE(look_event != NULL);
			BOOST_REQUIRE_EQUAL(look_event->robot_subset().size(), 1);
			active_robot = look_event->robot_subset().front();
			
			// second event should be a compute event for the same robot
			next_event = asg.get_next_event();
			ComputeEvent * compute_event = dynamic_cast<ComputeEvent*>(next_event.get());
			BOOST_REQUIRE(compute_event != NULL);
			BOOST_CHECK_EQUAL(compute_event->robot_subset().size(), 1);
			BOOST_CHECK_EQUAL(active_robot, compute_event->robot_subset().front());

			// third event should be a handle requests event with no request (since we did not update)
			next_event = asg.get_next_event();
			HandleRequestsEvent * handle_request_event = dynamic_cast<HandleRequestsEvent*>(next_event.get());
			BOOST_REQUIRE(handle_request_event != NULL);
			BOOST_CHECK_EQUAL(handle_request_event->requests().size(),0);

			//active robot must not be activated since last cycle
			BOOST_CHECK(activated_robots.find(active_robot) == activated_robots.end());
			activated_robots.insert(active_robot);
		}
	}



}

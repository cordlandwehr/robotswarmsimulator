/*
 * atomic_semisynchronous_asg_test.cc
 *
 *  Created on: Apr 8, 2009
 *      Author: craupach
 */

#include "../../ActivationSequenceGenerators/atomic_semisynchronous_asg.h"

#include "../../Model/world_information.h"
#include "../../Events/look_event.h"
#include "../../Events/compute_event.h"
#include "../../Events/handle_requests_event.h"
#include "../../Events/event.h"
#include "../../Model/robot.h"
#include "../../SimulationControl/history.h"
#include "../Fixtures/simple_world_fixture.h"

#include <iostream>

BOOST_FIXTURE_TEST_CASE(atomic_semisynchronous_asg_smoke_test, SimpleWorldFixture)
{
	AtomicSemisynchronousASG asg(517);
	asg.initialize(*history,robots);

	// first event should be at time 0
	BOOST_CHECK_EQUAL(asg.get_time_of_next_event(), 0);

	// first event should be a look event for one robots
	boost::shared_ptr<Event> next_event = asg.get_next_event();
	LookEvent * look_event = dynamic_cast<LookEvent*>(next_event.get());
	BOOST_REQUIRE(look_event != NULL);
	BOOST_CHECK_EQUAL(look_event->robot_subset().size(), 1);

	// second event should be a compute event for the same robot at time 0
	next_event = asg.get_next_event();
	ComputeEvent * compute_event = dynamic_cast<ComputeEvent*>(next_event.get());
	BOOST_REQUIRE(compute_event != NULL);
	BOOST_CHECK_EQUAL(compute_event->robot_subset().size(), 1);

	// third event should be a handle requests event with no request (since we did not update)
	next_event = asg.get_next_event();
	HandleRequestsEvent * handle_request_event = dynamic_cast<HandleRequestsEvent*>(next_event.get());
	BOOST_REQUIRE(handle_request_event != NULL);
	BOOST_CHECK_EQUAL(handle_request_event->requests().size(),0);

	// next event should be at time 1
	BOOST_CHECK_EQUAL(asg.get_time_of_next_event(), 1);
	next_event = asg.get_next_event();
	look_event = dynamic_cast<LookEvent*>(next_event.get());
	BOOST_CHECK(look_event != NULL);
	BOOST_CHECK_EQUAL(look_event->robot_subset().size(), 1);


}

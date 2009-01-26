#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../ActivationSequenceGenerators/asynchronous_asg.h"
#include "../../Model/world_information.h"
#include "../../Events/look_event.h"
#include "../../Events/compute_event.h"
#include "../../Events/handle_requests_event.h"
#include "../../Events/event.h"
#include "../../Model/robot.h"
#include "../../SimulationControl/history.h"
#include "../Fixtures/simple_world_fixture.h"
#include "asynchronous_asg_test_accessor.h"

#include <iostream>

/*
 * Test: Generate 4 Events with participation probability 1.0
 * Expected Results:
 *   - events: look, compute, handle request, look
 *   - each event is for all robots (except handle request which should have 0 requests)
 */
BOOST_FIXTURE_TEST_CASE(asynchronous_asg_smoke_test, SimpleWorldFixture)
{

	// seed = 67581, participation probability 1.0, lamda 0.5
	AsynchronousASG asynchronous_asg(67581, 1.0, 0.5);
	asynchronous_asg.initialize(*history,robots);

	AsynchronousASGTestAccessor asg_accessor(asynchronous_asg);

	// all robots should be looking robots by now
	BOOST_CHECK_EQUAL(robots.size(), asg_accessor.looking_robots().size());

	// since participation probability is 1.0 this should behave like a synchronous asg
	// (except timing of events)
	boost::shared_ptr<Event> next_event = asynchronous_asg.get_next_event();
	LookEvent * look_event = dynamic_cast<LookEvent*>(next_event.get());
	BOOST_CHECK(look_event != NULL);
	BOOST_CHECK_EQUAL(look_event->robot_subset().size(), robots.size());
	BOOST_CHECK_EQUAL(next_event->time(), 0); // first event for time 0
	int cur_time = next_event->time();

	// second event should be a compute event for all robots
	next_event = asynchronous_asg.get_next_event();
	ComputeEvent * compute_event = dynamic_cast<ComputeEvent*>(next_event.get());
	BOOST_CHECK(look_event != NULL);
	BOOST_CHECK_EQUAL(compute_event->robot_subset().size(), robots.size());
	BOOST_CHECK_GT(next_event->time(), cur_time);
	cur_time = next_event->time();

	// third event should be a handle requests event with no request (since we did not update)
	next_event = asynchronous_asg.get_next_event();
	HandleRequestsEvent * handle_request_event = dynamic_cast<HandleRequestsEvent*>(next_event.get());
	BOOST_CHECK(handle_request_event != NULL);
	BOOST_CHECK_EQUAL(handle_request_event->requests().size(), 0); // no actual requests!
	BOOST_CHECK_GT(next_event->time(), cur_time);
	cur_time = next_event->time();

	// fourth event should be a look event again
	next_event = asynchronous_asg.get_next_event();
	look_event = dynamic_cast<LookEvent*>(next_event.get());
	BOOST_CHECK(look_event != NULL);
	BOOST_CHECK_EQUAL(look_event->robot_subset().size(), robots.size());
	BOOST_CHECK_GT(next_event->time(), cur_time);
}

/*
 * Test: Generate 2 Events with participation probability 0.0
 * Expected Results:
 *   - events: look, look
 *   - each event is for 0 robots
 *   - all robots remain looking robots
 */
BOOST_FIXTURE_TEST_CASE(asynchronous_asg_robot_strike_test, SimpleWorldFixture)
{

	// seed = 67581, participation probability 1.0, lamda 0.5
	AsynchronousASG asynchronous_asg(67581, 0.0, 0.5);
	asynchronous_asg.initialize(*history,robots);

	AsynchronousASGTestAccessor asg_accessor(asynchronous_asg);

	// all robots should be looking robots by now
	BOOST_CHECK_EQUAL(robots.size(), asg_accessor.looking_robots().size());

	// since participation probability is 0.0 no robots should be choosen
	boost::shared_ptr<Event> next_event = asynchronous_asg.get_next_event();
	LookEvent * look_event = dynamic_cast<LookEvent*>(next_event.get());
	BOOST_CHECK(look_event != NULL);
	BOOST_CHECK_EQUAL(look_event->robot_subset().size(), 0);
	BOOST_CHECK_EQUAL(next_event->time(), 0); // first event for time 0
	int cur_time = next_event->time();

	// all robots should still be looking robots
	BOOST_CHECK_EQUAL(robots.size(), asg_accessor.looking_robots().size());

	next_event = asynchronous_asg.get_next_event();
	look_event = dynamic_cast<LookEvent*>(next_event.get());
	BOOST_CHECK(look_event != NULL);
	BOOST_CHECK_EQUAL(look_event->robot_subset().size(), 0);
	BOOST_CHECK_GT(next_event->time(), cur_time);

	// all robots should still be looking robots
	BOOST_CHECK_EQUAL(robots.size(), asg_accessor.looking_robots().size());
}

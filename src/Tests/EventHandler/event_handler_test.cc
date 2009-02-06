#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Events/look_event.h"
#include "../../Events/handle_requests_event.h"
#include "../../Events/event.h"
#include "../../Requests/request.h"
#include "../../Requests/position_request.h"
#include "../../SimulationControl/history.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../../Views/view_factory.h"
#include "../../Views/view.h"
#include "../../SimulationKernel/robot_control.h"
#include "test_simulation_listener.h"

#include "../Fixtures/simple_world_fixture.h"

#include <iostream>

/*
 *  Test: pass an empty look event for t = 5
 *  Expected Results:
 *    WorldInformation extrapolated to t = 5
 *    Last WorldInformation  at t = 5
 *    All Listeners Update Functions called once
 *    All Listeners last event is look event at t = 5
 *    All listeners last world information is the new one at t = 5
 *    Robot B is at (1.0, 0.5, 3.0)
 */
BOOST_FIXTURE_TEST_CASE(event_handler_test, SimpleWorldFixture)
{
	// Test set up: EventHandler and Listeners
	boost::shared_ptr<AbstractViewFactory> view_factory;
	view_factory.reset(new ViewFactory<View> ());

	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	EventHandler event_handler(history, robot_control);


	boost::shared_ptr<TestSimulationListener> listener_a;
	listener_a.reset(new TestSimulationListener());

	boost::shared_ptr<TestSimulationListener> listener_b;
	listener_b.reset(new TestSimulationListener());

	event_handler.register_listener(listener_a);
	event_handler.register_listener(listener_b);

	// Test Set up: Constructing Event and passing it to the handler
	boost::shared_ptr<LookEvent> look_event;
	look_event.reset(new LookEvent(5));
	event_handler.handle_event(look_event);

	// newest world information is for t = 5
	BOOST_CHECK_EQUAL(history->get_newest().time(), 5);

	// all listeners update functions called once
	BOOST_CHECK_EQUAL(listener_a->times_update_called(), 1);
	BOOST_CHECK_EQUAL(listener_b->times_update_called(), 1);

	// all listeners last event is look event at t = 5
	BOOST_CHECK_EQUAL(listener_a->last_event()->time(), 5);
	BOOST_CHECK_EQUAL(listener_b->last_event()->time(), 5);

	// all listeners world information is at t = 5
	BOOST_CHECK_EQUAL(listener_a->last_world_information().time(), 5);
	BOOST_CHECK_EQUAL(listener_b->last_world_information().time(), 5);

	// robot b is at (1.0, 0.5, 3.0)
	BOOST_CHECK_CLOSE(robot_data_b->position()(kXCoord), 1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(kYCoord), 0.5, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(kZCoord), 3.0, 0.1);
}

/*
 *  Test: pass an handle requests event for t = 7 containing a position request for robot_a
 *  Fixture: SimpleWorldFixture
 *  Expected Results:
 *    New WorldInformation generated
 *    Last WorldInformation at t = 7
 *    All Listeners Update Functions called twice
 *    All Listeners last event is handle requests event at t = 7
 *    All listeners last world information is the new one at t = 7
 *    Robot Cs Position is extrapolated to (1.0 + 0.5 * 1.0 * 7^2, ..., ...) according to s(t) = 0.5 * a * t^2
 *    Robot Cs Velocity is extrapolated to (7.0, 7.0, 7.0)
 *    Robot Bs Position is extrapolated to (1.0 + 7, 0.5, 3.0) = (8.0, 0.5, 3.0)
 *    Robot As Position remains at (0,0,0)
 *    Handle method for position requests called once in the event handler
 *    New WorldInformation should contain both robots with the same order
 *    GetAccordingRobotData should work for both robots and the new world information
 */
BOOST_FIXTURE_TEST_CASE(abstract_event_handler_extrapolation_test, SimpleWorldFixture) {
	// Test set up : EventHandler and Listener
	boost::shared_ptr<AbstractViewFactory> view_factory;
	view_factory.reset(new ViewFactory<View> ());

	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	EventHandler event_handler(history, robot_control);


	boost::shared_ptr<TestSimulationListener> listener_a;
	listener_a.reset(new TestSimulationListener());

	boost::shared_ptr<TestSimulationListener> listener_b;
	listener_b.reset(new TestSimulationListener());

	event_handler.register_listener(listener_a);
	event_handler.register_listener(listener_b);

	// Test set up: Constructing the Event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event;
	handle_requests_event.reset(new HandleRequestsEvent(7));

	boost::shared_ptr<Vector3d> new_position;
	new_position.reset(new Vector3d());
	new_position->insert_element(kXCoord,1.0);
	new_position->insert_element(kYCoord,2.0);
	new_position->insert_element(kZCoord,3.0);

	boost::shared_ptr<PositionRequest> position_request;
	position_request.reset(new PositionRequest(*robot_a, new_position));
	handle_requests_event->add_to_requests(position_request);

	// Pass the Event to the Handler
	event_handler.handle_event(handle_requests_event);

	// handle_position_request was called once
	// BOOST_CHECK_EQUAL(event_handler.calls_position_request(),1);

	// newest world information is for t = 7
	BOOST_CHECK_EQUAL(history->get_newest().time(), 7);

	// all listeners update functions called once
	BOOST_CHECK_EQUAL(listener_a->times_update_called(), 1);
	BOOST_CHECK_EQUAL(listener_b->times_update_called(), 1);

	// all listeners last event is hande request event at t = 7
	BOOST_CHECK_EQUAL(listener_a->last_event()->time(), 7);
	BOOST_CHECK_EQUAL(listener_b->last_event()->time(), 7);

	// all listeners world information is the new one at t = 7
	BOOST_CHECK_EQUAL(listener_a->last_world_information().time(), 7);
	BOOST_CHECK_EQUAL(listener_b->last_world_information().time(), 7);

	// new world information contains the number of robots...
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().size(), 3);

	// get robot datas in the new world_information
	RobotData new_robot_data_a = listener_a->last_world_information().get_according_robot_data(robot_a->id());
	RobotData new_robot_data_b = listener_a->last_world_information().get_according_robot_data(robot_b->id());
	RobotData new_robot_data_c = listener_a->last_world_information().get_according_robot_data(robot_c->id());

	// new world information contains new robot data objects. Old one should still contain the old ones.
	BOOST_CHECK_NE(robot_data_a.get(), &new_robot_data_a);
	BOOST_CHECK_NE(robot_data_b.get(), &new_robot_data_b);
	BOOST_CHECK_NE(robot_data_b.get(), &new_robot_data_c);
	BOOST_CHECK_EQUAL(robot_data_a.get(),
	                  &(initial_world_information->get_according_robot_data(robot_a->id())));


	// OLD robot_data_a is still at (0.0, 0.0, 0.0) since its the old object
	BOOST_CHECK_CLOSE(robot_data_a->position()(kXCoord), 0.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_a->position()(kYCoord), 0.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_a->position()(kZCoord), 0.0, 0.1);

	// OLD robot_data_b is  still at (1.0, 0.5, 3.0) since its the old object
	BOOST_CHECK_CLOSE(robot_data_b->position()(kXCoord), 1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(kYCoord), 0.5, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(kZCoord), 3.0, 0.1);

	// OLD robot_data_c is still at (1.0, 1.0, 1.0) since its the old object
	BOOST_CHECK_CLOSE(robot_data_c->position()(kXCoord), 1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_c->position()(kYCoord), 1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_c->position()(kZCoord), 1.0, 0.1);

	// NEW robot_data_a is still at (0.0, 0.0, 0.0) (no velocity, no acceleration)
	BOOST_CHECK_CLOSE(new_robot_data_a.position()(kXCoord), 0.0, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_a.position()(kYCoord), 0.0, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_a.position()(kZCoord), 0.0, 0.1);

	// NEW robot_data_b is now at (8.0, 0.5, 3.0)
	BOOST_CHECK_CLOSE(new_robot_data_b.position()(kXCoord), 8.0, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_b.position()(kYCoord), 0.5, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_b.position()(kZCoord), 3.0, 0.1);

	// NEW robot c is now at about (1.0 + 0.5 * 1.0 * 7^2,
	//                              1.0 + 0.5 * 1.0 * 7^2,
	//                              1.0 + 0.5 * 1.0 * 7^2)
	// for acceleration a: s(t) = 0.5 * a * t^2.
	BOOST_CHECK_CLOSE(new_robot_data_c.position()(kXCoord), 25.5, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_c.position()(kYCoord), 25.5, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_c.position()(kZCoord), 25.5, 0.1);

	// NEW robot c has a velocity of (7.0, 7.0, 7.0)
	BOOST_CHECK_CLOSE(new_robot_data_c.velocity()(kXCoord), 7.0, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_c.velocity()(kYCoord), 7.0, 0.1);
	BOOST_CHECK_CLOSE(new_robot_data_c.velocity()(kZCoord), 7.0, 0.1);
}

/*
 * Test: pass an event for t = 3 then another event for a past time ( t = 1 )
 * Fixture: SimpleWorldFixture
 * Expected Results: An invalid argument exception is thrown
 */
BOOST_FIXTURE_TEST_CASE(abstract_event_handler_past_event_test, SimpleWorldFixture) {
	// Test set up: EventHandler and Listeners
	boost::shared_ptr<AbstractViewFactory> view_factory;
	view_factory.reset(new ViewFactory<View> ());

	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	EventHandler event_handler(history, robot_control);


	boost::shared_ptr<TestSimulationListener> listener_a;
	listener_a.reset(new TestSimulationListener());

	boost::shared_ptr<TestSimulationListener> listener_b;
	listener_b.reset(new TestSimulationListener());

	event_handler.register_listener(listener_a);
	event_handler.register_listener(listener_b);

	// Test set up: Constructing first event and passing it to the handler
	boost::shared_ptr<LookEvent> look_event;
	look_event.reset(new LookEvent(3));
	event_handler.handle_event(look_event);

	// constructing the second event for past time t = 1
	boost::shared_ptr<LookEvent> look_event_b;
	look_event_b.reset(new LookEvent(1));

	// Expect disappointment
	BOOST_CHECK_THROW(event_handler.handle_event(look_event_b), std::invalid_argument);
}


/*
 * Test: Pass two Handle Requests Events
 * Expected Results: ...
 *
 */

/*
 * Test: pass an nonexisting request
 * Expected Results: An exception is thrown
 */

/*
 * Test: pass an nonexisting event
 * Expected Results: An exception is thrown
 */
BOOST_FIXTURE_TEST_CASE(abstract_event_handler_invalid_event_test, SimpleWorldFixture) {
	// Test set up: EventHandler and Listeners
	boost::shared_ptr<AbstractViewFactory> view_factory;
	view_factory.reset(new ViewFactory<View> ());

	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	EventHandler event_handler(history, robot_control);


	boost::shared_ptr<TestSimulationListener> listener_a;
	listener_a.reset(new TestSimulationListener());

	boost::shared_ptr<TestSimulationListener> listener_b;
	listener_b.reset(new TestSimulationListener());

	event_handler.register_listener(listener_a);
	event_handler.register_listener(listener_b);

	// TODO(craupach) repair test
	// constructing the second event for past time t = 1
	// boost::shared_ptr<ThePigsCanFlyEvent> invalid_event;
	// invalid_event.reset(new ThePigsCanFlyEvent(1));

	// Expect disappointment
	// BOOST_CHECK_THROW(event_handler.handle_event(invalid_event), std::invalid_argument);
}

/*
 * Test: check if obstacles / marker information get copied correctly
 * TODO(craupach) add obstacles and markers to the fixture.
 */

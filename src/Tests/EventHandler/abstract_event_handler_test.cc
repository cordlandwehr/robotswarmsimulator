#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

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
#include "../../Utilities/vector3d.h"
#include "../../Views/view_factory.h"
#include "../../Views/view.h"
#include "../../SimulationKernel/robot_control.h"
#include "test_event_handler.h"
#include "test_simulation_listener.h"

#include "../Fixtures/simple_world_fixture.h"

BOOST_FIXTURE_TEST_CASE(abstract_event_handler_test, SimpleWorldFixture)
{
	boost::shared_ptr<AbstractViewFactory> view_factory;
	view_factory.reset(new ViewFactory<View> ());

	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5));
	TestEventHandler event_handler(history, robot_control);


	boost::shared_ptr<TestSimulationListener> listener_a;
	listener_a.reset(new TestSimulationListener());

	boost::shared_ptr<TestSimulationListener> listener_b;
	listener_b.reset(new TestSimulationListener());

	event_handler.register_listener(listener_a);
	event_handler.register_listener(listener_b);

	/*
	 *  Test: pass an empty look event for t = 5
	 *  Expected Results:
	 *    No new WorldInformation generated
	 *    Last WorldInformation still at t = 0
	 *    All Listeners Update Functions called once
	 *    All Listeners last event is look event at t = 5
	 *    All listeners last world information is the old one at t = 0
	 *    Robot B is at (1.0, 0.5, 3.0)
	 */
	boost::shared_ptr<LookEvent> look_event;
	look_event.reset(new LookEvent(5));
	event_handler.handle_event(look_event);

	// newest world information is still for t = 0
	BOOST_CHECK_EQUAL(history->get_newest().time(), 0);

	// all listeners update functions called once
	BOOST_CHECK_EQUAL(listener_a->times_update_called(), 1);
	BOOST_CHECK_EQUAL(listener_b->times_update_called(), 1);

	// all listeners last event is look event at t = 5
	BOOST_CHECK_EQUAL(listener_a->last_event()->time(), 5);
	BOOST_CHECK_EQUAL(listener_b->last_event()->time(), 5);

	// all listeners world information is the old one at t = 0
	BOOST_CHECK_EQUAL(listener_a->last_world_information().time(), 0);
	BOOST_CHECK_EQUAL(listener_b->last_world_information().time(), 0);

	// robot b is at (1.0, 0.5, 3.0)
	BOOST_CHECK_CLOSE(robot_data_b->position()(0), 1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(1), 0.5, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(2), 3.0, 0.1);

	/*
	 *  Test: pass an handle requests event for t = 7 containing a position request for robot a
	 *  Expected Results:
	 *    New WorldInformation generated
	 *    Last WorldInformation at t = 7
	 *    All Listeners Update Functions called twice
	 *    All Listeners last event is handle requests event at t = 7
	 *    All listeners last world information is the new one at t = 7
	 *    Robot Bs Position is extrapolated to (1.0 + 7, 0.5, 3.0) = (8.0, 0.5, 3.0)
	 *    Robot As Position remains at (0,0,0)
	 *    Handle method for position requests called once in the event handler
	 *    New WorldInformation should contain both robots with the same order
	 *    GetAccordingRobotData should work for both robots and the new world information
	 */

	boost::shared_ptr<HandleRequestsEvent> handle_requests_event;
	handle_requests_event.reset(new HandleRequestsEvent(7));

	boost::shared_ptr<Vector3d> new_position;
	new_position.reset(new Vector3d());
	new_position->insert_element(0,1.0);
	new_position->insert_element(1,2.0);
	new_position->insert_element(2,3.0);

	boost::shared_ptr<PositionRequest> position_request;
	position_request.reset(new PositionRequest(robot_a, new_position));
	handle_requests_event->add_to_requests(position_request);

	event_handler.handle_event(handle_requests_event);

	// newest world information is for t = 7
	BOOST_CHECK_EQUAL(history->get_newest().time(), 7);

	// all listeners update functions called twice
	BOOST_CHECK_EQUAL(listener_a->times_update_called(), 2);
	BOOST_CHECK_EQUAL(listener_b->times_update_called(), 2);

	// all listeners last event is hande request event at t = 7
	BOOST_CHECK_EQUAL(listener_a->last_event()->time(), 7);
	BOOST_CHECK_EQUAL(listener_b->last_event()->time(), 7);

	// all listeners world information is the new one at t = 7
	BOOST_CHECK_EQUAL(listener_a->last_world_information().time(), 7);
	BOOST_CHECK_EQUAL(listener_b->last_world_information().time(), 7);

	// robot b is at (8.0, 0.5, 3.0)
	BOOST_CHECK_CLOSE(robot_data_b->position()(0), 8.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(1), 0.5, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(2), 3.0, 0.1);

	// robot a is at (0.0, 0.0, 0.0)
	BOOST_CHECK_CLOSE(robot_data_a->position()(0), 0.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_a->position()(1), 0.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_a->position()(2), 0.0, 0.1);

	// handle_position_request was called once
	BOOST_CHECK_EQUAL(event_handler.calls_position_request(),1);

	// new world information contains the same robots...
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().size(), 2);

	// ... with the same positions
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().at(0)->position()(0),
	                  robot_data_a->position()(0));
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().at(0)->position()(1),
		                  robot_data_a->position()(1));
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().at(0)->position()(2),
		                  robot_data_a->position()(2));
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().at(1)->position()(0),
	                  robot_data_b->position()(0));
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().at(1)->position()(1),
		                  robot_data_b->position()(1));
	BOOST_CHECK_EQUAL(listener_a->last_world_information().robot_data().at(1)->position()(2),
		                  robot_data_b->position()(2));

	// get_according_robot for both world information still works
	// TODO(craupach) calls to get_according_robot_data are very complicated right now
	// TODO(craupach) set IDs in the fixture. How is this supposed to be done?
	BOOST_CHECK_EQUAL(robot_data_a.get(),
	                  &(initial_world_information->get_according_robot_data(robot_a->id().get())));
	BOOST_CHECK_EQUAL(robot_data_a.get(),
	                  &(listener_a->last_world_information().get_according_robot_data(robot_a->id().get())));

	/*
	 * Test: Pass another Handle Requests Event
	 * Expected Results: ...
	 *
	 */

	/*
	 * Test: pass an event for a past time ( t = 1 )
	 * Expected Results: don't know yet
	 */

	/*
	 * Test: pass an nonexisting request
	 * Expected Results: An exception is thrown
	 */

	/*
	 * Test: pass an nonexisting event
	 * Expected Results: An exception is thrown
	 */

	/*
	 * Test: check if obstacles / marker information get copied correctly
	 * TODO(craupach) add obstacles and markers to the fixture.
	 */
}

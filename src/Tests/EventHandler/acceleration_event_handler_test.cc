/*
 * acceleration_event_handler_test.cc
 *
 *  Created on: 20.01.2009
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Events/handle_requests_event.h"

#include "../../EventHandlers/exact_acceleration_event_handler.h"

#include "../../Requests/acceleration_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/robot_control.h"

#include "../../Utilities/vector3d.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_world_fixture.h"

/*
 * Test: pass an event (t=3) with a AccelerationRequest (10., 1., -1.2) for robot_a to a AccelerationEventHandler
 * Expected Results:
 *   - acceleration of robot_a changed according to the request
 */
BOOST_FIXTURE_TEST_CASE(acceleration_event_handler_test_acceleration_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History
	ExactAccelerationEventHandler event_handler(history, robot_control);

	// construction of acceleration request
	boost::shared_ptr<Vector3d> new_acceleration(new Vector3d);
	(*new_acceleration)(0) = 10.;
	(*new_acceleration)(1) = 1.;
	(*new_acceleration)(2) = -1.2;
	boost::shared_ptr<AccelerationRequest> acceleration_request(new AccelerationRequest(robot_a, new_acceleration));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(3));
	handle_requests_event->add_to_requests(acceleration_request);

	/* BEGIN: InitialTest (no event processed yet)
	 * - only one world information (at time 0)
	 * - robot_a still at (0., 0., 0.) with velocity (0., 0., 0.) and acceleration (0., 0., 0.)
	 */
	const RobotData& robot_data_before = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 0);
	BOOST_CHECK_CLOSE(robot_data_before.position()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.position()(1), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.position()(2), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.velocity()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.velocity()(1), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.velocity()(2), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.acceleration()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.acceleration()(1), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.acceleration()(2), 0., 0.1);
	/* END: InitialTest */

	// handle event for acceleration request
	event_handler.handle_event(handle_requests_event);

	/* BEGIN: EventHandledTest1
	 * - world information at time 3
	 * - robot_a has acceleration (10., 1., -1.2)
	 */
	const RobotData& robot_data_after = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 3);
	BOOST_CHECK_CLOSE(robot_data_after.acceleration()(0),  10.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.acceleration()(1),   1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.acceleration()(2), - 1.2, 0.1);
	/* END: EventHandledTest */
}

/*
 * Test: Set a different local coordinate system for robot a and issue a acceleration request for it.
 * Expected Results:
 *   Robot a's acceleration changes according to its local coordinate system.
 */
BOOST_FIXTURE_TEST_CASE(acceleration_event_handler_test_local_coordinate_system, SimpleWorldFixture) {
	// setting up event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5));
	ExactAccelerationEventHandler event_handler(history, robot_control);

	// build a local coordinate axes for a robot
	boost::shared_ptr<Vector3d> x_axis(new Vector3d());
	x_axis->insert_element(kXCoord, 2.0);
	x_axis->insert_element(kYCoord, 1.0);
	x_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> y_axis(new Vector3d());
	y_axis->insert_element(kXCoord, 0.0);
	y_axis->insert_element(kYCoord, 2.0);
	y_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> z_axis(new Vector3d());
	z_axis->insert_element(kXCoord, 0.0);
	z_axis->insert_element(kYCoord, 0.0);
	z_axis->insert_element(kZCoord, 1.5);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	              boost::shared_ptr<Vector3d> > axes(x_axis, y_axis, z_axis);

	robot_data_a->set_coordinate_system_axis(axes);

	// build a acceleration request
	boost::shared_ptr<Vector3d> new_acceleration(new Vector3d);
	(*new_acceleration)(kXCoord) =  1.0;
	(*new_acceleration)(kYCoord) = -0.5;
	(*new_acceleration)(kZCoord) =  1.5;
	boost::shared_ptr<AccelerationRequest> acceleration_request(new AccelerationRequest(robot_a, new_acceleration));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(1));
	handle_requests_event->add_to_requests(acceleration_request);

	// handling the event
	event_handler.handle_event(handle_requests_event);

	// checking new acceleration of robot_a: should be (2.0, 0.0, 2.25)
	const RobotData& robot_data_after = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 1);
	BOOST_CHECK_CLOSE(robot_data_after.acceleration()(kXCoord),  2.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.acceleration()(kYCoord), 0.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.acceleration()(kZCoord), 2.25, 0.1);
}
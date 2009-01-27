/*
 * velocity_event_handler_test.cc
 *
 *  Created on: 20.01.2009
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Events/handle_requests_event.h"

#include "../../EventHandlers/exact_velocity_event_handler.h"

#include "../../Requests/velocity_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/robot_control.h"

#include "../../Utilities/vector3d.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_world_fixture.h"

/*
 * Test: pass an event (t=3) with a VelocityRequest (0., 1., -0.5) for robot_a to a VelocityEventHandler
 *       pass an empty event for t=8
 * Expected Results:
 *   - t=3: velocity of robot_a changed according to the request
 *   - t=8: velocity still (0., 1., -0.5), position = (0., 5., -2.5)
 */
BOOST_FIXTURE_TEST_CASE(velocity_event_handler_test_velocity_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History
	ExactVelocityEventHandler event_handler(history, robot_control);

	// construction of velocity request
	boost::shared_ptr<Vector3d> new_velocity(new Vector3d);
	(*new_velocity)(0) = 0.;
	(*new_velocity)(1) = 1.;
	(*new_velocity)(2) = -0.5;
	boost::shared_ptr<VelocityRequest> velocity_request(new VelocityRequest(*robot_a, new_velocity));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(3));
	handle_requests_event->add_to_requests(velocity_request);

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

	// handle event for velocity request
	event_handler.handle_event(handle_requests_event);

	/* BEGIN: EventHandledTest1
	 * - world information at time 3
	 * - robot_a at (0., 0., 0.) with velocity (0., 1., -0.5)
	 */
	const RobotData& robot_data_after = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 3);
	BOOST_CHECK_CLOSE(robot_data_after.position()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(1), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(2), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(1), 1., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(2), -0.5, 0.1);
	/* END: EventHandledTest */

	// construction and handling of empty handle_requests_event
	handle_requests_event.reset(new HandleRequestsEvent(8));
	event_handler.handle_event(handle_requests_event);

	/* BEGIN: EventHandledTest2
	 * - world information at time 8
	 * - robot_a at (0., 5., -2.5) with velocity (0., 1., -0.5)
	 */
	const RobotData& robot_data_after2 = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 8);
	BOOST_CHECK_CLOSE(robot_data_after2.position()(0),  0. , 0.1);
	BOOST_CHECK_CLOSE(robot_data_after2.position()(1),  5. , 0.1);
	BOOST_CHECK_CLOSE(robot_data_after2.position()(2), -2.5, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after2.velocity()(0),  0. , 0.1);
	BOOST_CHECK_CLOSE(robot_data_after2.velocity()(1),  1. , 0.1);
	BOOST_CHECK_CLOSE(robot_data_after2.velocity()(2), -0.5, 0.1);
	/* END: EventHandledTest */
}

/*
 * Test: Set a different local coordinate system for robot a and issue a velocity request for it.
 * Expected Results:
 *   Robot a's velocity changes according to its local coordinate system.
 */
BOOST_FIXTURE_TEST_CASE(velocity_event_handler_test_local_coordinate_system, SimpleWorldFixture) {
	// setting up event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5));
	ExactVelocityEventHandler event_handler(history, robot_control);

	// build a local coordinate axes for a robot
	boost::shared_ptr<Vector3d> x_axis(new Vector3d());
	x_axis->insert_element(kXCoord, 3.0);
	x_axis->insert_element(kYCoord, 0.0);
	x_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> y_axis(new Vector3d());
	y_axis->insert_element(kXCoord, 0.0);
	y_axis->insert_element(kYCoord, 2.0);
	y_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> z_axis(new Vector3d());
	z_axis->insert_element(kXCoord, 0.0);
	z_axis->insert_element(kYCoord, 0.0);
	z_axis->insert_element(kZCoord, 0.5);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	              boost::shared_ptr<Vector3d> > axes(x_axis, y_axis, z_axis);

	robot_data_a->set_coordinate_system_axis(axes);

	// build a velocity request
	boost::shared_ptr<Vector3d> new_velocity(new Vector3d);
	(*new_velocity)(kXCoord) =  1.0;
	(*new_velocity)(kYCoord) = -0.5;
	(*new_velocity)(kZCoord) =  1.5;
	boost::shared_ptr<VelocityRequest> velocity_request(new VelocityRequest(*robot_a, new_velocity));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(4));
	handle_requests_event->add_to_requests(velocity_request);

	// handling the event
	event_handler.handle_event(handle_requests_event);

	// checking new velocity of robot_a: should be (3.0, -1.0, 0.75)
	const RobotData& robot_data_after = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 4);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(kXCoord),  3.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(kYCoord), -1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(kZCoord), 0.75, 0.1);
}

/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * velocity_event_handler_test.cc
 *
 *  Created on: 20.01.2009
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Events/handle_requests_event.h"

#include "../../EventHandlers/event_handler.h"
#include "../../EventHandlers/vector_request_handler.h"

#include "../../Requests/velocity_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../Utilities/vector_arithmetics.h"

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
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	EventHandler event_handler(history, robot_control);
	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(5, 0.0, *history));
	event_handler.set_velocity_request_handler(request_handler_vel);


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
	const RobotData& robot_data_before = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 0);
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
	boost::shared_ptr<TimePoint> time_point(new TimePoint());
	event_handler.handle_event(handle_requests_event, *time_point);
	history->insert(time_point);

	/* BEGIN: EventHandledTest1
	 * - world information at time 3
	 * - robot_a at (0., 0., 0.) with velocity (0., 1., -0.5)
	 */
	const RobotData& robot_data_after = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 3);
	BOOST_CHECK_CLOSE(robot_data_after.position()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(1), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(2), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(1), 1., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(2), -0.5, 0.1);
	/* END: EventHandledTest */

	// construction and handling of empty handle_requests_event
	handle_requests_event.reset(new HandleRequestsEvent(8));
	boost::shared_ptr<TimePoint> time_point_b(new TimePoint());
	event_handler.handle_event(handle_requests_event, *time_point_b);
	history->insert(time_point_b);

	/* BEGIN: EventHandledTest2
	 * - world information at time 8
	 * - robot_a at (0., 5., -2.5) with velocity (0., 1., -0.5)
	 */
	const RobotData& robot_data_after2 = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 8);
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
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	EventHandler event_handler(history, robot_control);
	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(5, 0.0, *history));
		event_handler.set_velocity_request_handler(request_handler_vel);

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
	boost::shared_ptr<TimePoint> time_point(new TimePoint());
	event_handler.handle_event(handle_requests_event, *time_point);
	history->insert(time_point);

	// checking new velocity of robot_a: should be (3.0, -1.0, 0.75)
	const RobotData& robot_data_after = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 4);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(kXCoord),  3.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(kYCoord), -1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.velocity()(kZCoord), 0.75, 0.1);
}

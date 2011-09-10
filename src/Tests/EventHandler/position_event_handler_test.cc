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
 * position_event_handler_test.cc
 *
 *  Created on: 16.01.2009
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Events/handle_requests_event.h"

#include "../../EventHandlers/event_handler.h"
#include "../../EventHandlers/vector_request_handler.h"

#include "../../Requests/position_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../Utilities/vector_arithmetics.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_world_fixture.h"


/*
 * Test: pass a PositionRequest for robot_a to a PositionEventHandler
 * Expected Results:
 *   - robot_a is at (-5, 0.1, 3.) after event is processed (was at (0., 0., 0.) before)
 */
BOOST_FIXTURE_TEST_CASE(positiont_event_handler_test_position_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History
	EventHandler event_handler(history, robot_control);
	boost::shared_ptr<VectorRequestHandler> request_handler_pos(new VectorRequestHandler(5, 0.0, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.;
	(*new_position)(1) = 0.1;
	(*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(4));
	handle_requests_event->add_to_requests(position_request);

	/* BEGIN: InitialTest (no event processed yet)
	 * - only one world information (at time 0)
	 * - robot_a still at (0., 0., 0.)
	 */
	const RobotData& robot_data_before = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 0);
	BOOST_CHECK_CLOSE(robot_data_before.position()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.position()(1), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.position()(2), 0., 0.1);
	/* END: InitialTest */

	// handle event for position request
	boost::shared_ptr<TimePoint> time_point(new TimePoint());
	event_handler.handle_event(handle_requests_event, *time_point);
	history->insert(time_point);

	/* BEGIN: EventHandledTest
	 * - world information at time 4
	 * - robot_a at (-5., 0.1, 3.)
	 */
	const RobotData& robot_data_after = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 4);
	BOOST_CHECK_CLOSE(robot_data_after.position()(0), -5., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(1), 0.1, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(2), 3., 0.1);
	/* END: EventHandledTest */
}

/*
 * Test: - pass a TestMarkerRequest(1) for robot_a to a PositionEventHandler
 *       - pass a TestMarkerRequest(2) for robot_a to the PositionEventHandler (as a second event)
 * Expected Results:
 *   - robot_a's marker information after first event is of correct type and carries integral value '1'
 *   - robot_a's marker information after second event is of correct type and carries integral value '2'
 *
 *   TODO(craupach) This test is not accurate. It should be rewriten after MarkerInformation has been
 *   changed
 *   TODO(peter) This test should (after having been rewritten) be moved to its own test file.
 */
BOOST_FIXTURE_TEST_CASE(positiont_event_handler_test_marker_test, SimpleWorldFixture) {
//	// setup of event handler
//	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
//	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5));
//	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History
//	ExactFullEventHandler event_handler(history, robot_control);
//
//	// construction of first marker request
//	boost::shared_ptr<MarkerInformation> new_marker_information(new TestMarkerInformation(1));
//	boost::shared_ptr<MarkerRequest> marker_request(new MarkerRequest(robot_a, new_marker_information));
//
//	// construction of first handle_requests_event
//	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(3));
//	handle_requests_event->add_to_requests(marker_request);
//
//	/* BEGIN: InitialTest (no event processed yet)
//	 * - only one world information (at time 0)
//	 * - robot_a has default marker information
//	 */
//	const RobotData& robot_data_before = history->get_newest().world_information().get_according_robot_data(robot_a->id());
//	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 0);
//	BOOST_CHECK_THROW(dynamic_cast<const TestMarkerInformation&>(robot_data_before.marker_information()), std::bad_cast);
//	/* END: InitialTest */
//
//	// handle first event for marker request
//	event_handler.handle_event(handle_requests_event);
//
//	/* BEGIN: EventHandledTest1
//	 * - world information at time 3
//	 * - robot_a has marker information of type TestMarkerInformation with value '1'
//	 */
//	const RobotData& robot_data_after1 = history->get_newest().world_information().get_according_robot_data(robot_a->id());
//	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 3);
//	BOOST_REQUIRE_NO_THROW(dynamic_cast<const TestMarkerInformation&>(robot_data_after1.marker_information()));
//	const TestMarkerInformation& marker_information_after1 = dynamic_cast<const TestMarkerInformation&>(robot_data_after1.marker_information());
//	BOOST_CHECK_EQUAL(marker_information_after1.value(), 1);
//	/* END: EventHandledTest1 */
//
//	// construction of second marker request
//	new_marker_information.reset(new TestMarkerInformation(2));
//	marker_request.reset(new MarkerRequest(robot_a, new_marker_information));
//
//	// construction of second handle_request_event
//	handle_requests_event.reset(new HandleRequestsEvent(6));
//	handle_requests_event->add_to_requests(marker_request);
//
//	// handle second event for marker request
//	event_handler.handle_event(handle_requests_event);
//
//	/* BEGIN: EventHandledTest2
//	 * - world information at time 6
//	 * - robot_a has marker information of type TestMarkerInformation with value '2'
//	 */
//	const RobotData& robot_data_after2 = history->get_newest().world_information().get_according_robot_data(robot_a->id());
//	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 6);
//	BOOST_REQUIRE_NO_THROW(dynamic_cast<const TestMarkerInformation&>(robot_data_after2.marker_information()));
//	const TestMarkerInformation& marker_information_after2 = dynamic_cast<const TestMarkerInformation&>(robot_data_after2.marker_information());
//	BOOST_CHECK_EQUAL(marker_information_after2.value(), 2);
//	/* END: EventHandledTest2 */
//
//	// construction and handling of third (empty) handle_request_event
//	handle_requests_event.reset(new HandleRequestsEvent(9));
//	event_handler.handle_event(handle_requests_event);
//
//	/* BEGIN: EventHandledTest3
//	 * - world information at time 9
//	 * - robot_a has marker information of type TestMarkerInformation with value '2'
//	 */
//	const RobotData& robot_data_after3 = history->get_newest().world_information().get_according_robot_data(robot_a->id());
//	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 9);
//	BOOST_REQUIRE_NO_THROW(dynamic_cast<const TestMarkerInformation&>(robot_data_after3.marker_information()));
//	const TestMarkerInformation& marker_information_after3 = dynamic_cast<const TestMarkerInformation&>(robot_data_after3.marker_information());
//	BOOST_CHECK_EQUAL(marker_information_after3.value(), 2);
//	/* END: EventHandledTest2 */
}

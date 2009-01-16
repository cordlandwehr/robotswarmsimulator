/*
 * position_event_handler_test.cc
 *
 *  Created on: 16.01.2009
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Events/handle_requests_event.h"

#include "../../EventHandlers/position_event_handler.h"

#include "../../Requests/position_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/robot_control.h"

#include "../../Utilities/vector3d.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_world_fixture.h"

/*
 * Test: pass a PositionRequest for robot_a to an PositionEventHandler
 * Expected Results:
 *   - robot_a is at (-5, 0.1, 3.) after event is processed (was at (0., 0., 0.) before)
 */
BOOST_FIXTURE_TEST_CASE(positiont_event_handler_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History
	PositionEventHandler event_handler(history, robot_control);

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.;
	(*new_position)(1) = 0.1;
	(*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(robot_a, new_position));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(4));
	handle_requests_event->add_to_requests(position_request);

	/* BEGIN: InitialTest (no event processed yet)
	 * - only one world information (at time 0)
	 * - robot_a still at (0., 0., 0.)
	 */
	const RobotData& robot_data_before = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 0);
	BOOST_CHECK_CLOSE(robot_data_before.position()(0), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.position()(1), 0., 0.1);
	BOOST_CHECK_CLOSE(robot_data_before.position()(2), 0., 0.1);
	/* END: InitialTest */

	// handle event for position request
	event_handler.handle_event(handle_requests_event);

	/* BEGIN: EventHandledTest
	 * - world information at time 4
	 * - robot_a still at (-5., 0.1, 3.)
	 */
	const RobotData& robot_data_after = history->get_newest().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(history->get_newest().time(), 4);
	BOOST_CHECK_CLOSE(robot_data_after.position()(0), -5., 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(1), 0.1, 0.1);
	BOOST_CHECK_CLOSE(robot_data_after.position()(2), 3., 0.1);
	/* END: EventHandledTest */
}

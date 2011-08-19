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
 * unreliable_event_handler_test.cc
 *
 *  Created on: 25.01.2009
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Events/handle_requests_event.h"

#include "../../EventHandlers/event_handler.h"
#include "../../EventHandlers/vector_request_handler.h"
#include "../../EventHandlers/marker_request_handler.h"


#include "../../Requests/marker_request.h"
#include "../../Requests/position_request.h"


#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../Utilities/vector_arithmetics.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_world_fixture.h"

#include "test_marker_request_handler.h"
#include "test_vector_request_handler.h"


/*
 * Test: pass several requests to an unreliable request handler with discard probability 1.0
 * Expected Results:
 *   - none of the events is handled
 */
BOOST_FIXTURE_TEST_CASE(unreliable_event_handler_test_discard_all_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	// create event handler
	unsigned int seed = 219684;
	double discard_probability = 1.0;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<TestVectorRequestHandler> request_handler_pos(new TestVectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<TestMarkerRequestHandler> request_handler_marker(new TestMarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	// construction of marker request
	boost::shared_ptr<MarkerInformation> new_marker_information(new MarkerInformation());
	boost::shared_ptr<MarkerRequest> marker_request(new MarkerRequest(*robot_a, new_marker_information));


	// construction and handling of several handle_requests_event
	int nr_requests = 100;
	for (int i=1; i<=nr_requests; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(marker_request);
		boost::shared_ptr<TimePoint> time_point(new TimePoint());
		event_handler.handle_event(handle_requests_event, *time_point);
		history->insert(time_point);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: NoneHandledTest
	 * - none event has been handled
	 */
	BOOST_CHECK_EQUAL(request_handler_marker->nr_handled(), 0);
	/* END: NoneHandledTest */
}


/*
 * Test: pass several requests to an unreliable request handler with discard probability 0.0
 * Expected Results:
 *   - all events are handled
 */
BOOST_FIXTURE_TEST_CASE(unreliable_event_handler_test_discard_none_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	// create event handler
	unsigned int seed = 23458;
	double discard_probability = 0.0;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<TestVectorRequestHandler> request_handler_pos(new TestVectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);	
	
	boost::shared_ptr<TestMarkerRequestHandler> request_handler_marker(new TestMarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	// construction of marker request
	boost::shared_ptr<MarkerInformation> new_marker_information(new MarkerInformation());
	boost::shared_ptr<MarkerRequest> marker_request(new MarkerRequest(*robot_a, new_marker_information));

	
	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.; (*new_position)(1) = 0.1; (*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction and handling of several handle_requests_event
	int nr_requests = 100;
	for (int i=1; i<=nr_requests ; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(marker_request);
		handle_requests_event->add_to_requests(position_request);
		boost::shared_ptr<TimePoint> time_point(new TimePoint());
		event_handler.handle_event(handle_requests_event, *time_point);
		history->insert(time_point);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: AllHandledTest
	 * - all events have been handled
	 */
	BOOST_CHECK_EQUAL(request_handler_pos->nr_handled(), nr_requests);
	BOOST_CHECK_EQUAL(request_handler_marker->nr_handled(), nr_requests);
	/* END: AllHandledTest */
}


/*
 * Test: pass several requests to an unreliable request handler with discard probability 0.25
 * Expected Results:
 *   - about 3/4 of the requests are handled
 */
BOOST_FIXTURE_TEST_CASE(unreliable_event_handler_test_discard_few_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	// create event handler
	unsigned int seed = 768334;
	double discard_probability = 0.25;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<TestVectorRequestHandler> request_handler_pos(new TestVectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<TestMarkerRequestHandler> request_handler_marker(new TestMarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	// construction of marker request
	boost::shared_ptr<MarkerInformation> new_marker_information(new MarkerInformation());
	boost::shared_ptr<MarkerRequest> marker_request(new MarkerRequest(*robot_a, new_marker_information));

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.; (*new_position)(1) = 0.1; (*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction and handling of several handle_requests_event
	int nr_requests = 50000;
	for (int i=1; i<=nr_requests ; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(marker_request);
		handle_requests_event->add_to_requests(position_request);
		boost::shared_ptr<TimePoint> time_point(new TimePoint());
		event_handler.handle_event(handle_requests_event, *time_point);
		history->insert(time_point);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: FewHandledTest
	 * - about 3/4 of the events have been handled (we allow an error of 2%)
	 */
	double estimated_nr = static_cast<double>(nr_requests) * 3./4.;
	BOOST_CHECK_CLOSE(static_cast<double>(request_handler_pos->nr_handled()), estimated_nr, 2.0);
	BOOST_CHECK_CLOSE(static_cast<double>(request_handler_marker->nr_handled()), estimated_nr, 2.0);
	/* END: FewHandledTest */
}


/*
 * Test: pass several requests to an unreliable request handler with discard probability 0.75
 * Expected Results:
 *   - about 1/4 of the requests are handled
 */
BOOST_FIXTURE_TEST_CASE(unreliable_event_handler_test_discard_many_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	// create event handler
	unsigned int seed = 34579;
	double discard_probability = 0.75;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<TestVectorRequestHandler> request_handler_pos(new TestVectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<TestMarkerRequestHandler> request_handler_marker(new TestMarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	// construction of marker request
	boost::shared_ptr<MarkerInformation> new_marker_information(new MarkerInformation());
	boost::shared_ptr<MarkerRequest> marker_request(new MarkerRequest(*robot_a, new_marker_information));

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.; (*new_position)(1) = 0.1; (*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction and handling of several handle_requests_event
	int nr_requests = 50000;
	for (int i=1; i<=nr_requests ; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(marker_request);
		handle_requests_event->add_to_requests(position_request);
		boost::shared_ptr<TimePoint> time_point(new TimePoint());
		event_handler.handle_event(handle_requests_event, *time_point);
		history->insert(time_point);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: ManyHandledTest
	 * - about 3/4 of the events have been handled (we allow an error of 2%)
	 */
	double estimated_nr = static_cast<double>(nr_requests) * 0.25;
	BOOST_CHECK_CLOSE(static_cast<double>(request_handler_pos->nr_handled()), estimated_nr, 2.0);
	BOOST_CHECK_CLOSE(static_cast<double>(request_handler_marker->nr_handled()), estimated_nr, 2.0);
	/* END: ManyHandledTest */
}

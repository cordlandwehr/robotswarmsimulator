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
#include "../../EventHandlers/type_change_request_handler.h"

#include "../../Requests/acceleration_request.h"
#include "../../Requests/marker_request.h"
#include "../../Requests/position_request.h"
#include "../../Requests/type_change_request.h"
#include "../../Requests/velocity_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/robot_control.h"

#include "../../Utilities/vector3d.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_world_fixture.h"


/*
 * Test: pass several requests to an unreliable request handler with discard probability 1.0
 * Expected Results:
 *   - none of the events is handled
 */
BOOST_FIXTURE_TEST_CASE(unreliable_event_handler_test_discard_all_test, SimpleWorldFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History

	// create event handler
	unsigned int seed = 219684;
	double discard_probability = 1.0;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<VectorRequestHandler> request_handler_acc(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_acceleration_request_handler(request_handler_acc);

	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_velocity_request_handler(request_handler_vel);

	boost::shared_ptr<VectorRequestHandler> request_handler_pos(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<MarkerRequestHandler> request_handler_marker(new MarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	boost::shared_ptr<TypeChangeRequestHandler> request_handler_type(new TypeChangeRequestHandler(seed, discard_probability, *history));
	event_handler.set_type_change_request_handler(request_handler_type);

	// construction of acceleration request
	boost::shared_ptr<Vector3d> new_acceleration(new Vector3d);
	(*new_acceleration)(0) = -3.; (*new_acceleration)(1) = -7.8; (*new_acceleration)(2) = 1.;
	boost::shared_ptr<AccelerationRequest> acceleration_request(new AccelerationRequest(*robot_a, new_acceleration));

	// TODO: marker request (marker_information not yet redesigned)

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.; (*new_position)(1) = 0.1; (*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction of type change request
	RobotType new_type;
	boost::shared_ptr<TypeChangeRequest> type_change_request(new TypeChangeRequest(*robot_a, new_type));

	// construction of velocity request
	boost::shared_ptr<Vector3d> new_velocity(new Vector3d);
		(*new_velocity)(0) = -5.; (*new_velocity)(1) = 0.1; (*new_velocity)(2) = 3.;
		boost::shared_ptr<VelocityRequest> velocity_request(new VelocityRequest(*robot_a, new_velocity));

	// construction and handling of several handle_requests_event
	int nr_requests = 100;
	for (int i=1; i<=nr_requests; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(acceleration_request);
		//handle_requests_event->add_to_requests(marker_request);
		handle_requests_event->add_to_requests(position_request);
		handle_requests_event->add_to_requests(type_change_request);
		handle_requests_event->add_to_requests(velocity_request);
		event_handler.handle_event(handle_requests_event);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: NoneHandledTest
	 * - none event has been handled
	 */
//	BOOST_CHECK_EQUAL(event_handler.calls_acceleration_request(), 0);
//	BOOST_CHECK_EQUAL(event_handler.calls_marker_request()      , 0);
//	BOOST_CHECK_EQUAL(event_handler.calls_position_request()    , 0);
//	BOOST_CHECK_EQUAL(event_handler.calls_type_change_request() , 0);
//	BOOST_CHECK_EQUAL(event_handler.calls_velocity_request()    , 0);
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
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History

	// create event handler
	unsigned int seed = 23458;
	double discard_probability = 0.0;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<VectorRequestHandler> request_handler_acc(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_acceleration_request_handler(request_handler_acc);

	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_velocity_request_handler(request_handler_vel);

	boost::shared_ptr<VectorRequestHandler> request_handler_pos(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<MarkerRequestHandler> request_handler_marker(new MarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	boost::shared_ptr<TypeChangeRequestHandler> request_handler_type(new TypeChangeRequestHandler(seed, discard_probability, *history));
	event_handler.set_type_change_request_handler(request_handler_type);

	// construction of acceleration request
	boost::shared_ptr<Vector3d> new_acceleration(new Vector3d);
	(*new_acceleration)(0) = -3.; (*new_acceleration)(1) = -7.8; (*new_acceleration)(2) = 1.;
	boost::shared_ptr<AccelerationRequest> acceleration_request(new AccelerationRequest(*robot_a, new_acceleration));

	// TODO: marker request (marker_information not yet redesigned)

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.; (*new_position)(1) = 0.1; (*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction of type change request
	RobotType new_type;
	boost::shared_ptr<TypeChangeRequest> type_change_request(new TypeChangeRequest(*robot_a, new_type));

	// construction of velocity request
	boost::shared_ptr<Vector3d> new_velocity(new Vector3d);
		(*new_velocity)(0) = -5.; (*new_velocity)(1) = 0.1; (*new_velocity)(2) = 3.;
		boost::shared_ptr<VelocityRequest> velocity_request(new VelocityRequest(*robot_a, new_velocity));

	// construction and handling of several handle_requests_event
	int nr_requests = 100;
	for (int i=1; i<=nr_requests ; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(acceleration_request);
		//handle_requests_event->add_to_requests(marker_request);
		handle_requests_event->add_to_requests(position_request);
		handle_requests_event->add_to_requests(type_change_request);
		handle_requests_event->add_to_requests(velocity_request);
		event_handler.handle_event(handle_requests_event);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: AllHandledTest
	 * - all events have been handled
	 */
//	BOOST_CHECK_EQUAL(event_handler.calls_acceleration_request(), nr_requests);
//	BOOST_CHECK_EQUAL(event_handler.calls_marker_request()      , nr_requests);
//	BOOST_CHECK_EQUAL(event_handler.calls_position_request()    , nr_requests);
//	BOOST_CHECK_EQUAL(event_handler.calls_type_change_request() , nr_requests);
//	BOOST_CHECK_EQUAL(event_handler.calls_velocity_request()    , nr_requests);
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
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History

	// create event handler
	unsigned int seed = 768334;
	double discard_probability = 0.25;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<VectorRequestHandler> request_handler_acc(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_acceleration_request_handler(request_handler_acc);

	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_velocity_request_handler(request_handler_vel);

	boost::shared_ptr<VectorRequestHandler> request_handler_pos(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<MarkerRequestHandler> request_handler_marker(new MarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	boost::shared_ptr<TypeChangeRequestHandler> request_handler_type(new TypeChangeRequestHandler(seed, discard_probability, *history));
	event_handler.set_type_change_request_handler(request_handler_type);

	// construction of acceleration request
	boost::shared_ptr<Vector3d> new_acceleration(new Vector3d);
	(*new_acceleration)(0) = -3.; (*new_acceleration)(1) = -7.8; (*new_acceleration)(2) = 1.;
	boost::shared_ptr<AccelerationRequest> acceleration_request(new AccelerationRequest(*robot_a, new_acceleration));

	// TODO: marker request (marker_information not yet redesigned)

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.; (*new_position)(1) = 0.1; (*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction of type change request
	RobotType new_type;
	boost::shared_ptr<TypeChangeRequest> type_change_request(new TypeChangeRequest(*robot_a, new_type));

	// construction of velocity request
	boost::shared_ptr<Vector3d> new_velocity(new Vector3d);
		(*new_velocity)(0) = -5.; (*new_velocity)(1) = 0.1; (*new_velocity)(2) = 3.;
		boost::shared_ptr<VelocityRequest> velocity_request(new VelocityRequest(*robot_a, new_velocity));

	// construction and handling of several handle_requests_event
	int nr_requests = 25000;
	for (int i=1; i<=nr_requests ; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(acceleration_request);
		//handle_requests_event->add_to_requests(marker_request);
		handle_requests_event->add_to_requests(position_request);
		handle_requests_event->add_to_requests(type_change_request);
		handle_requests_event->add_to_requests(velocity_request);
		event_handler.handle_event(handle_requests_event);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: FewHandledTest
	 * - about 3/4 of the events have been handled (we allow an error of 2%)
	 */
	double estimated_nr = static_cast<double>(nr_requests) * 3./4.;
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_acceleration_request()), estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_marker_request())      , estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_position_request())    , estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_type_change_request()) , estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_velocity_request())    , estimated_nr, 2.0);
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
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, 5, *initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History

	// create event handler
	unsigned int seed = 34579;
	double discard_probability = 0.75;
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<VectorRequestHandler> request_handler_acc(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_acceleration_request_handler(request_handler_acc);

	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_velocity_request_handler(request_handler_vel);

	boost::shared_ptr<VectorRequestHandler> request_handler_pos(new VectorRequestHandler(seed, discard_probability, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<MarkerRequestHandler> request_handler_marker(new MarkerRequestHandler(seed, discard_probability, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	boost::shared_ptr<TypeChangeRequestHandler> request_handler_type(new TypeChangeRequestHandler(seed, discard_probability, *history));
	event_handler.set_type_change_request_handler(request_handler_type);

	// construction of acceleration request
	boost::shared_ptr<Vector3d> new_acceleration(new Vector3d);
	(*new_acceleration)(0) = -3.; (*new_acceleration)(1) = -7.8; (*new_acceleration)(2) = 1.;
	boost::shared_ptr<AccelerationRequest> acceleration_request(new AccelerationRequest(*robot_a, new_acceleration));

	// TODO: marker request (marker_information not yet redesigned)

	// construction of position request
	boost::shared_ptr<Vector3d> new_position(new Vector3d);
	(*new_position)(0) = -5.; (*new_position)(1) = 0.1; (*new_position)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, new_position));

	// construction of type change request
	RobotType new_type;
	boost::shared_ptr<TypeChangeRequest> type_change_request(new TypeChangeRequest(*robot_a, new_type));

	// construction of velocity request
	boost::shared_ptr<Vector3d> new_velocity(new Vector3d);
		(*new_velocity)(0) = -5.; (*new_velocity)(1) = 0.1; (*new_velocity)(2) = 3.;
		boost::shared_ptr<VelocityRequest> velocity_request(new VelocityRequest(*robot_a, new_velocity));

	// construction and handling of several handle_requests_event
	int nr_requests = 25000;
	for (int i=1; i<=nr_requests ; i++) {
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(acceleration_request);
		//handle_requests_event->add_to_requests(marker_request);
		handle_requests_event->add_to_requests(position_request);
		handle_requests_event->add_to_requests(type_change_request);
		handle_requests_event->add_to_requests(velocity_request);
		event_handler.handle_event(handle_requests_event);
		history->get_oldest_unused(); // consume, to make sure that we do not block
	}

	/* BEGIN: ManyHandledTest
	 * - about 3/4 of the events have been handled (we allow an error of 2%)
	 */
	double estimated_nr = static_cast<double>(nr_requests) * 1./4.;
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_acceleration_request()), estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_marker_request())      , estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_position_request())    , estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_type_change_request()) , estimated_nr, 2.0);
//	BOOST_CHECK_CLOSE(static_cast<double>(event_handler.calls_velocity_request())    , estimated_nr, 2.0);
	/* END: ManyHandledTest */
}

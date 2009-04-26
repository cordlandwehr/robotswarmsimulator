/*
 * inaccurate_event_handler_test.cc
 *
 *  Created on: 25.01.2009
 *      Author: peter
 */

#include <cmath>

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Events/handle_requests_event.h"

#include "../../EventHandlers/event_handler.h"
#include "../../EventHandlers/vector_request_handler.h"

#include "../../Requests/acceleration_request.h"
#include "../../Requests/marker_request.h"
#include "../../Requests/position_request.h"
#include "../../Requests/type_change_request.h"
#include "../../Requests/velocity_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../ComputationalGeometry/coord_converter.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../../Utilities/VectorModifiers/vector_randomizer.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_world_fixture.h"


/*
 * Test: pass several position requests to an inaccurate event handler
 * Expected Results:
 *   - the inaccuracy conforms to the respective inaccuracy parameters
 */
BOOST_FIXTURE_TEST_CASE(inaccurat_event_handler_test_position_test, SimpleWorldFixture) {
	using CoordConverter::local_to_global;

	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History

	// create event handler
	EventHandler event_handler(history, robot_control);

	// create inaccurate position request handler
	unsigned int seed = 678321;
	double standard_deviation = 0.221;
	boost::shared_ptr<VectorRandomizer> vector_modifier(new VectorRandomizer(seed, standard_deviation));
	boost::shared_ptr<VectorRequestHandler> request_handler(new VectorRequestHandler(0, 0., *history));
	request_handler->add_vector_modifier(vector_modifier);
	event_handler.set_position_request_handler(request_handler);

	// construction of the request
	boost::shared_ptr<Vector3d> requested_vector(new Vector3d);
	(*requested_vector)(0) = -5.;
	(*requested_vector)(1) = 0.1;
	(*requested_vector)(2) = 3.;
	boost::shared_ptr<PositionRequest> position_request(new PositionRequest(*robot_a, requested_vector));

	// construction and handling of several handle_requests_event
	int nr_requests = 25000;
	Vector3d mean_vector = boost::numeric::ublas::zero_vector<double>(3);
	Vector3d deviation_vector = boost::numeric::ublas::zero_vector<double>(3);
	for (int i=1; i<=nr_requests; i++) {
		// get global target point
		RobotData robot_data_before = history->get_newest().world_information().get_according_robot_data(robot_a->id());
		boost::shared_ptr<Vector3d> target_point = local_to_global(*requested_vector, robot_data_before.position(),
																   robot_data_before.coordinate_system_axis());

		// handle event
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(position_request);
		event_handler.handle_event(handle_requests_event);
		history->get_oldest_unused(); // consume, to make sure that we do not block

		// get reached point
		RobotData robot_data_after = history->get_newest().world_information().get_according_robot_data(robot_a->id());
		const Vector3d& reached_point = robot_data_after.position();

		// computation of mean displacement and deviation
		const Vector3d& difference = reached_point - *target_point;
		mean_vector += difference;
		deviation_vector += element_prod(difference, difference);
	}
	mean_vector /= nr_requests;
	deviation_vector  /= nr_requests;
	deviation_vector(0) = sqrt(deviation_vector(0));
	deviation_vector(1) = sqrt(deviation_vector(1));
	deviation_vector(2) = sqrt(deviation_vector(2));

	/* BEGIN: ConformsToInaccuracy
	 * - every component should conform to the N(0, standard_deviation) distribution
	 */
	BOOST_CHECK_SMALL(mean_vector(0), 5E-3);
	BOOST_CHECK_SMALL(mean_vector(1), 5E-3);
	BOOST_CHECK_SMALL(mean_vector(2), 5E-3);
	BOOST_CHECK_CLOSE(deviation_vector(0), standard_deviation, 1.);
	BOOST_CHECK_CLOSE(deviation_vector(1), standard_deviation, 1.);
	BOOST_CHECK_CLOSE(deviation_vector(2), standard_deviation, 1.);
	/* END: ConformsToInaccuracy */
}


/*
 * Test: pass several acceleration requests to an inaccurate full event handler
 * Expected Results:
 *   - the inaccuracy conforms to the respective inaccuracy parameters
 */
BOOST_FIXTURE_TEST_CASE(inaccurat_event_handler_test_acceleration_test, SimpleWorldFixture) {
	using CoordConverter::local_to_global;

	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History

	// create event handler
	EventHandler event_handler(history, robot_control);

	// create event handler
	unsigned int seed = 898193;
	double standard_deviation = 0.489;
	boost::shared_ptr<VectorRandomizer> vector_modifier(new VectorRandomizer(seed, standard_deviation));
	boost::shared_ptr<VectorRequestHandler> request_handler(new VectorRequestHandler(0, 0., *history));
	request_handler->add_vector_modifier(vector_modifier);
	event_handler.set_acceleration_request_handler(request_handler);

	// construction of the request
	boost::shared_ptr<Vector3d> requested_vector(new Vector3d);
	(*requested_vector)(0) = -15.;
	(*requested_vector)(1) = 15.1;
	(*requested_vector)(2) = -345.;
	boost::shared_ptr<AccelerationRequest> request(new AccelerationRequest(*robot_b, requested_vector));

	// get requested vector in global coordinates
	boost::numeric::ublas::zero_vector<double> zero(3);
	boost::shared_ptr<Vector3d> global_req_vector = local_to_global(*requested_vector, zero,
																	robot_data_b->coordinate_system_axis());

	// construction and handling of several handle_requests_event
	int nr_requests = 25000;
	Vector3d mean_vector = boost::numeric::ublas::zero_vector<double>(3);
	Vector3d deviation_vector = boost::numeric::ublas::zero_vector<double>(3);
	for (int i=1; i<=nr_requests; i++) {
		// handle event
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(request);
		event_handler.handle_event(handle_requests_event);
		history->get_oldest_unused(); // consume, to make sure that we do not block

		// computation of mean displacement and deviation
		const RobotData& robot_data_after = history->get_newest().world_information().get_according_robot_data(robot_b->id());
		const Vector3d& robot_vector_after = robot_data_after.acceleration();
		Vector3d difference = robot_vector_after - *global_req_vector;
		mean_vector += difference;
		deviation_vector += element_prod(difference, difference);
	}
	mean_vector /= nr_requests;
	deviation_vector  /= nr_requests;
	deviation_vector(0) = sqrt(deviation_vector(0));
	deviation_vector(1) = sqrt(deviation_vector(1));
	deviation_vector(2) = sqrt(deviation_vector(2));

	/* BEGIN: ConformsToInaccuracy
	 * - every component should conform to the N(0, standard_deviation) distribution
	 */
	BOOST_CHECK_SMALL(mean_vector(0), 5E-3);
	BOOST_CHECK_SMALL(mean_vector(1), 5E-3);
	BOOST_CHECK_SMALL(mean_vector(2), 5E-3);
	BOOST_CHECK_CLOSE(deviation_vector(0), standard_deviation, 1.);
	BOOST_CHECK_CLOSE(deviation_vector(1), standard_deviation, 1.);
	BOOST_CHECK_CLOSE(deviation_vector(2), standard_deviation, 1.);
	/* END: ConformsToInaccuracy */
}


/*
 * Test: pass several velocity requests to an inaccurate full event handler
 * Expected Results:
 *   - the inaccuracy conforms to the respective inaccuracy parameters
 */
BOOST_FIXTURE_TEST_CASE(inaccurat_event_handler_test_velocity_test, SimpleWorldFixture) {
	using CoordConverter::local_to_global;

	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	// TODO(peter) 'new RobotControl(view_factory, history->size)' would be better ==> add size() method to History

	// create event handler
	EventHandler event_handler(history, robot_control);

	// create event handler
	unsigned int seed = 23;
	double standard_deviation = 0.42;
	boost::shared_ptr<VectorRandomizer> vector_modifier(new VectorRandomizer(seed, standard_deviation));
	boost::shared_ptr<VectorRequestHandler> request_handler(new VectorRequestHandler(0, 0., *history));
	request_handler->add_vector_modifier(vector_modifier);
	event_handler.set_velocity_request_handler(request_handler);

	// construction of the request
	boost::shared_ptr<Vector3d> requested_vector(new Vector3d);
	(*requested_vector)(0) = 123.;
	(*requested_vector)(1) = 9.1;
	(*requested_vector)(2) = 27.9033;
	boost::shared_ptr<VelocityRequest> request(new VelocityRequest(*robot_b, requested_vector));

	// get requested vector in global coordinates
	boost::numeric::ublas::zero_vector<double> zero(3);
	boost::shared_ptr<Vector3d> global_req_vector = local_to_global(*requested_vector, zero,
																	robot_data_b->coordinate_system_axis());

	// construction and handling of several handle_requests_event
	int nr_requests = 25000;
	Vector3d mean_vector = boost::numeric::ublas::zero_vector<double>(3);
	Vector3d deviation_vector = boost::numeric::ublas::zero_vector<double>(3);
	for (int i=1; i<=nr_requests; i++) {
		// handle event
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(request);
		event_handler.handle_event(handle_requests_event);
		history->get_oldest_unused(); // consume, to make sure that we do not block

		// computation of mean displacement and deviation
		const RobotData& robot_data_after = history->get_newest().world_information().get_according_robot_data(robot_b->id());
		const Vector3d& robot_vector_after = robot_data_after.velocity();
		Vector3d difference = robot_vector_after - *global_req_vector;
		mean_vector += difference;
		deviation_vector += element_prod(difference, difference);
	}
	mean_vector /= nr_requests;
	deviation_vector  /= nr_requests;
	deviation_vector(0) = sqrt(deviation_vector(0));
	deviation_vector(1) = sqrt(deviation_vector(1));
	deviation_vector(2) = sqrt(deviation_vector(2));

	/* BEGIN: ConformsToInaccuracy
	 * - every component should conform to the N(0, standard_deviation) distribution
	 */
	BOOST_CHECK_SMALL(mean_vector(0), 5E-3);
	BOOST_CHECK_SMALL(mean_vector(1), 5E-3);
	BOOST_CHECK_SMALL(mean_vector(2), 5E-3);
	BOOST_CHECK_CLOSE(deviation_vector(0), standard_deviation, 1.);
	BOOST_CHECK_CLOSE(deviation_vector(1), standard_deviation, 1.);
	BOOST_CHECK_CLOSE(deviation_vector(2), standard_deviation, 1.);
	/* END: ConformsToInaccuracy */
}

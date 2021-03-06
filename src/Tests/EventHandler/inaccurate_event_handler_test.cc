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

#include "../../Requests/marker_request.h"
#include "../../Requests/position_request.h"

#include "../../SimulationControl/history.h"

#include "../../SimulationKernel/uniform_robot_control.h"

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
		boost::shared_ptr<Vector3d> target_point = requested_vector; //TODO asetzer if anything is defunc, THIS change might be the reason
		//= local_to_global(*requested_vector, robot_data_before.position(),
		//														   robot_data_before.coordinate_system_axis());

		// handle event
		boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(i));
		handle_requests_event->add_to_requests(position_request);
		boost::shared_ptr<TimePoint> time_point(new TimePoint());
		event_handler.handle_event(handle_requests_event, *time_point);
		history->insert(time_point);
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



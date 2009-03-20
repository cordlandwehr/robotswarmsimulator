/*
 * acceleration_cog_robot.cc
 *
 *  Created on: Feb 28, 2009
 *      Author: craupach
 */

#include "acceleration_cog_robot.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/acceleration_request.h"
#include "../Views/view.h"
#include "../Utilities/vector_arithmetics.h"

std::set<boost::shared_ptr<Request> > AccelerationCOGRobot::compute() {
		boost::shared_ptr<View> view = view_;

		// compute the center of gravity of all robots
		std::set<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);
		Vector3d result = view->get_position(*this, id());
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			result += view->get_position(*this, cur_id);
		}
		result /= visible_robots.size() + 1;


		// cut the resulting vector back to max_acceleration_
		result = result - view->get_position(*this, id());
	    double rescale = max_acceleration_ / boost::numeric::ublas::norm_2(result);
		if (rescale < 1) {
			result *= rescale;
		}

		// build acceleration request to move towards cog
		boost::shared_ptr<Vector3d> result_ptr(new Vector3d(result));
		boost::shared_ptr<AccelerationRequest> request(new AccelerationRequest(*this, result_ptr));

		// adjust max_acceleration_
		max_acceleration_ *= 0.9;

		// insert request into request set
		std::set<boost::shared_ptr<Request> > result_set;
		result_set.insert(request);

		return result_set;
}

/*
 * velocity_cog_robot.cc
 *
 *  Created on: Feb 28, 2009
 *      Author: craupach
 */

#include "velocity_cog_robot.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/velocity_request.h"

#include "../Views/view.h"

#include "../Utilities/vector_arithmetics.h"

std::set<boost::shared_ptr<Request> > VelocityCOGRobot::compute() {
		boost::shared_ptr<View> view = view_.lock();

		// compute the center of gravity of all robots
		std::set<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);
		Vector3d result = view->get_position(*this, id());
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			result += view->get_position(*this, cur_id);
		}
		result /= visible_robots.size() + 1;

		// build velocity request to move towards cog
		boost::shared_ptr<Vector3d> result_ptr(new Vector3d(result -  view->get_position(*this, id())));
		boost::shared_ptr<VelocityRequest> request(new VelocityRequest(*this, result_ptr));

		// insert request into request set
		std::set<boost::shared_ptr<Request> > result_set;
		result_set.insert(request);

		return result_set;
}

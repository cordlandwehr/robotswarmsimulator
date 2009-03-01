/*
 * miniball_robot.cc
 *
 *  Created on: Mar 1, 2009
 *      Author: craupach
 */

#include "miniball_robot.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/position_request.h"
#include "../Views/view.h"
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/miniball.h"
#include "../Utilities/miniball.cc"
#include "../Utilities/miniball_b.h"
#include "../Utilities/miniball_b.cc"

namespace {
	Point<3> vector3d_to_point(Vector3d vector) {
		Point<3> point;
		point[0] = vector(0);
		point[1] = vector(1);
		point[2] = vector(2);
		return point;
	}

	Vector3d point_to_vector3d(Point<3> point) {
		Vector3d vector;
		vector(0) = point[0];
		vector(1) = point[1];
		vector(2) = point[2];
		return vector;
	}
}


std::set<boost::shared_ptr<Request> > MiniballRobot::compute() {
		boost::shared_ptr<View> view = view_.lock();
		Miniball<3> miniball;
		miniball.check_in(vector3d_to_point(view->get_position(*this, id())));
		// compute the miniball of all visible robots
		std::set<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			miniball.check_in(vector3d_to_point(view->get_position(*this, cur_id)));
		}
		miniball.build();
		boost::shared_ptr<Vector3d> result_ptr(new Vector3d(point_to_vector3d(miniball.center())));
		// create a position request to the center of the miniball
		boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, result_ptr));

		std::set<boost::shared_ptr<Request> > result_set;
		result_set.insert(request);

		return result_set;
}

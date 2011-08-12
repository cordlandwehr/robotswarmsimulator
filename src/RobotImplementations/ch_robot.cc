/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

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
 * ch_robot.h
 *
 *  Created on: Mar 20, 2009
 *      Author: martinah
 */

#include "ch_robot.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/position_request.h"
#include "../Views/view.h"
#include "../Utilities/vector_arithmetics.h"
#include "../ComputationalGeometry/ch_algorithms.h"

std::set<boost::shared_ptr<Request> > CHRobot::compute() {
		boost::shared_ptr<View> view = view_;

		// compute the center of gravity of all robots
		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);
		Vector3d result = view->get_position(*this, id());

		//get positions of visible robots
		std::vector<Vector3d> points_of_vis_robots;
		points_of_vis_robots.push_back(view->get_position(*this, id()));
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			points_of_vis_robots.push_back(view->get_position(*this, cur_id));
		}

		//compute cog of given set of points
		result = CHAlgorithms::compute_cog_of_ch_of_points(points_of_vis_robots);
		result /= visible_robots.size()+1;

		// build position request to move towards cog of convex hull
		boost::shared_ptr<Vector3d> result_ptr(new Vector3d(result -  view->get_position(*this, id())));
		boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, result_ptr));

		std::set<boost::shared_ptr<Request> > result_set;
		result_set.insert(request);

		return result_set;
}

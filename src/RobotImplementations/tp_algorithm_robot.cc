/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 * tp_algorithm_robot.cc
 *
 *  Created on: Apr 10, 2009
 *      Author: craupach
 */

#include "tp_algorithm_robot.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/position_request.h"

#include "../Views/view.h"

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/console_output.h"

#include "../ComputationalGeometry/point_algorithms.h"

std::set<boost::shared_ptr<Request> > TPAlgorithmRobot::compute() {
		boost::shared_ptr<View> view = view_;

		// compute a std::vector with all robot positions in it
		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);
		Vector3d own_position = view->get_position(*this, id());
		std::vector<Vector3d> positions;
		positions.push_back(own_position);
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			positions.push_back(view->get_position(*this, cur_id));
		}

		// compute the target point
		Vector3d target_point = compute_target_point(own_position,positions);

		// build a position request for given target point
		boost::shared_ptr<Vector3d> target_point_ptr(new Vector3d(target_point));
		boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, target_point_ptr));
		std::set<boost::shared_ptr<Request> > result_set;
		result_set.insert(request);

		return result_set;
}

Vector3d TPAlgorithmRobot::compute_target_point(const Vector3d & own_position, const std::vector<Vector3d>& positions) {
	switch(type_) {
	case cog:
		return PointAlgorithms::compute_COG(positions);
	case cminball:
		return PointAlgorithms::compute_CMinBall(positions);
	case cbox:
		return PointAlgorithms::compute_CMinBox(positions);
	case maxline:
		return PointAlgorithms::compute_MaxLine(0, positions);
	case midfar:
		return PointAlgorithms::compute_MidFar(own_position, positions);
	case median:
		return PointAlgorithms::compute_MED(positions);
	case rminrect:
		return PointAlgorithms::compute_RMinRect(source_of_randomness, positions);
	default:
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error) << "tried to invoke unkown tp algorithm";
		return Vector3d();
	}
}

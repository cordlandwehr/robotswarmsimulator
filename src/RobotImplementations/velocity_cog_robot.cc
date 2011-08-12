/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
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
		boost::shared_ptr<View> view = view_;

		// compute the center of gravity of all robots
		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);
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

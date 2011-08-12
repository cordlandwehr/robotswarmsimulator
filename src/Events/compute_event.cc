/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
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
 * compute_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "compute_event.h"

#include <list>
#include <boost/smart_ptr.hpp>

using boost::shared_ptr;

void ComputeEvent::add_to_robot_subset(shared_ptr<Robot> new_robot) {
	robot_subset_.push_back(new_robot);
}

const std::list<shared_ptr<Robot> >& ComputeEvent::robot_subset() const {
	return robot_subset_;
}

void ComputeEvent::add_to_requests(shared_ptr<const Request> new_request) {
	requests_.push_back(new_request);
}

const std::list<shared_ptr<const Request> >& ComputeEvent::requests() const {
	return requests_;
}

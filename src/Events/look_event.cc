/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
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
 * look_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "look_event.h"

void LookEvent::add_to_robot_subset(boost::shared_ptr<Robot> new_robot) {
	robot_subset_.push_back(new_robot);
}

const std::list<boost::shared_ptr<Robot> >& LookEvent::robot_subset() const {
	return robot_subset_;
}

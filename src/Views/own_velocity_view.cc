/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

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
 * own_velocity_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_velocity_view.h"

OwnVelocityView::OwnVelocityView() {

}

OwnVelocityView::~OwnVelocityView() {

}

Vector3d OwnVelocityView::get_own_velocity(const RobotData& robot) const {
	return robot.velocity();
}

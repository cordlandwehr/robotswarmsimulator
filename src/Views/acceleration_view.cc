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
 * acceleration_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "acceleration_view.h"

AccelerationView::AccelerationView() {

}

AccelerationView::~AccelerationView() {
}

Vector3d AccelerationView::get_others_acceleration(const RobotData& robot) const {
	return robot.acceleration();
}

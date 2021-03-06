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
 * position_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef POSITION_VIEW_H_
#define POSITION_VIEW_H_

#include "own_position_view.h"

class PositionView : public virtual OwnPositionView {
public:
	PositionView();
	virtual ~PositionView();

protected:
	virtual Vector3d get_robot_position(const RobotData& robot) const;
	virtual Vector3d get_obstacle_position(const Obstacle& obstacle) const;
	virtual Vector3d get_marker_position(const WorldObject& marker) const;
	virtual Vector3d get_edge_position(const Edge& edge) const;
	virtual Vector3d get_message_position(const Message& message) const;
};

#endif /* POSITION_VIEW_H_ */

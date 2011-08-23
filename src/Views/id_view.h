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
 * id_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef ID_VIEW_H_
#define ID_VIEW_H_

#include "own_id_view.h"

class IdView : public virtual OwnIdView {
public:
	IdView();
	virtual ~IdView();
protected:
	virtual std::size_t get_robot_id(const RobotData& robot) const;
	virtual std::size_t get_obstacle_id(const Obstacle& obstacle) const;
	virtual std::size_t get_marker_id(const WorldObject& marker) const;
	virtual std::size_t get_edge_id(const Edge& edge) const;
	virtual std::size_t get_message_id(const Message& message) const;
};

#endif /* ID_VIEW_H_ */

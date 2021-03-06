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
 * marker_information_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef MARKER_INFORMATION_VIEW_H_
#define MARKER_INFORMATION_VIEW_H_

#include "own_marker_view.h"

class MarkerInformationView: public virtual OwnMarkerView {
public:
	MarkerInformationView();
	virtual ~MarkerInformationView();

protected:
	virtual MarkerInformation get_robots_marker_information(const RobotData& robot) const;
	virtual MarkerInformation get_obstacles_marker_information(const Obstacle& obstacle) const;
	virtual MarkerInformation get_markers_marker_information(const WorldObject& marker) const;
	virtual MarkerInformation get_edge_marker_information(const Edge& edge) const;
	virtual MarkerInformation get_message_marker_information(const Message& message) const;
};

#endif /* MARKER_INFORMATION_VIEW_H_ */

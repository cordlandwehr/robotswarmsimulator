/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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
 * neighbor_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#include "neighbor_view.h"
#include "octree_utilities.h"
#include "../Model/robot.h"

NeighborView::NeighborView(unsigned seen_objects_count) : seen_objects_count_(seen_objects_count) {

}

NeighborView::~NeighborView() {

}

unsigned NeighborView::seen_objects_count() const {
	return seen_objects_count_;
}

std::vector<View::RobotRef> NeighborView::get_visible_robots(const RobotData& robot) const {

	return OctreeUtilities::get_nearest_robots( octree(), robot.position(), boost::static_pointer_cast<RobotIdentifier>(robot.id()), std::size_t(seen_objects_count() ) );
}
std::vector<View::ObstacleRef> NeighborView::get_visible_obstacles(const RobotData& robot) const {

	return OctreeUtilities::get_nearest_obstacles(octree(), robot.position(), std::size_t(seen_objects_count() ) );
}
std::vector<View::MarkerRef> NeighborView::get_visible_markers(const RobotData& robot) const {

	return OctreeUtilities::get_nearest_markers( octree(), robot.position(), std::size_t(seen_objects_count() ) );
}

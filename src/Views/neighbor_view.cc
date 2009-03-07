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

std::set<View::RobotRef> NeighborView::get_visible_robots(const RobotData& robot) const {

	return OctreeUtilities::get_nearest_robots( octree(), robot.position(), boost::static_pointer_cast<RobotIdentifier>(robot.id()), std::size_t(seen_objects_count() ) );
}
std::set<View::ObstacleRef> NeighborView::get_visible_obstacles(const RobotData& robot) const {

	return OctreeUtilities::get_nearest_obstacles(octree(), robot.position(), std::size_t(seen_objects_count() ) );
}
std::set<View::MarkerRef> NeighborView::get_visible_markers(const RobotData& robot) const {

	return OctreeUtilities::get_nearest_markers( octree(), robot.position(), std::size_t(seen_objects_count() ) );
}

/*
 * neighbor_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#include "neighbor_view.h"

NeighborView::NeighborView(unsigned seen_objects_count) : seen_objects_count_(seen_objects_count) {

}

NeighborView::~NeighborView() {

}

std::set<View::RobotRef> NeighborView::get_visible_robots(const RobotData& robot) const {
	//TODO: implement
	return std::set<RobotRef>();
}
std::set<View::ObstacleRef> NeighborView::get_visible_obstacles(const RobotData& robot) const {
	//TODO: implement
	return std::set<ObstacleRef>();
}
std::set<View::MarkerRef> NeighborView::get_visible_markers(const RobotData& robot) const {
	//TODO: implement
	return std::set<MarkerRef>();
}

/*
 * own_position_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "position_view.h"
#include "../Model/obstacle.h"

PositionView::PositionView() {

}

PositionView::~PositionView() {
}

Vector3d PositionView::get_robot_position(const RobotData& robot) const {
	return robot.position();
}
Vector3d PositionView::get_obstacle_position(const Obstacle& obstacle) const {
	return obstacle.position();
}
Vector3d PositionView::get_marker_position(const WorldObject& marker) const {
	return marker.position();
}

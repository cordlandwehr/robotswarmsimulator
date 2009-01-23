/*
 * id_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "id_view.h"
#include "../Model/identifier.h"
#include "../Model/obstacle.h"

IdView::IdView() {


}

IdView::~IdView() {

}

std::size_t IdView::get_robot_id(const RobotData& robot) const {
	return get_id(robot.id());
}
std::size_t IdView::get_obstacle_id(const Obstacle& obstacle) const {
	return get_id(obstacle.id());
}
std::size_t IdView::get_marker_id(const WorldObject& marker) const {
	return get_id(marker.id());
}

/*
 * spheric_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#include "spheric_view.h"


SphericView::SphericView(double view_radius) : view_radius_(view_radius) {

}

SphericView::~SphericView() {
}

std::set<View::RobotRef> SphericView::get_visible_robots(const RobotData& robot) const {
	//TODO: implement
	return std::set<RobotRef>();
}
std::set<View::ObstacleRef> SphericView::get_visible_obstacles(const RobotData& robot) const {
	//TODO: implement
	return std::set<ObstacleRef>();
}
std::set<View::MarkerRef> SphericView::get_visible_markers(const RobotData& robot) const {
	//TODO: implement
	return std::set<MarkerRef>();
}

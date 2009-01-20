/*
 * spheric_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */
#include "../Model/world_information.h"
#include "octree.h"
#include "spheric_view.h"


SphericView::SphericView(double view_radius) : view_radius_(view_radius) {

}

SphericView::~SphericView() {
}

void SphericView::init(const WorldInformation & world_information ){
	View::init(world_information);
	octree_.reset(new Octree(8,view_radius_ * 2.0));
	octree_->create_tree(world_information.markers(),
						 world_information.obstacles(),
	 					 world_information.robot_data() );

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

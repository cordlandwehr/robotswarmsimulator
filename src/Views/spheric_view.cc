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
 * spheric_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */
#include "../Model/world_information.h"
#include "octree.h"
#include "octree_utilities.h"
#include "spheric_view.h"


SphericView::SphericView(double view_radius) : view_radius_(view_radius) {

}

SphericView::~SphericView() {
}

double SphericView::view_radius() const {
	return view_radius_;
}

void SphericView::init(const boost::shared_ptr<WorldInformation>& world_information ){
	View::init(world_information);
	octree_.reset(new Octree(int(std::ceil(std::log((double)world_information->robot_data().size() ))),view_radius_ * 2.0));
	octree_->create_tree(world_information->markers(),
						 world_information->obstacles(),
	 					 world_information->robot_data() );

}

std::vector<View::RobotRef> SphericView::get_visible_robots(const RobotData& robot) const {

	return OctreeUtilities::get_visible_robots_by_radius(octree(),robot.position(), view_radius(), robot );
}
std::vector<View::ObstacleRef> SphericView::get_visible_obstacles(const RobotData& robot) const {

	return OctreeUtilities::get_visible_obstacles_by_radius(octree(), robot.position(), view_radius() );
}
std::vector<View::MarkerRef> SphericView::get_visible_markers(const RobotData& robot) const {

	return OctreeUtilities::get_visible_markers_by_radius(octree(),robot.position() , view_radius() );
}

const double SphericView::get_view_radius() const {
	return view_radius_;
}
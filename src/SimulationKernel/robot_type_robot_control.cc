/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>

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
 * robot_type_robot_control.cc
 *
 *  Created on: 25.02.2009
 *      Author: Daniel
 */

#include <SimulationKernel/robot_type_robot_control.h>

#include <algorithm>

#include "../Views/abstract_view_factory.h"
#include "../Views/view.h"
#include "../Model/robot.h"
#include "../Model/world_information.h"

RobotTypeRobotControl::RobotTypeRobotControl(const boost::array<boost::shared_ptr<AbstractViewFactory>,kRobotTypeCount>& view_factories, std::size_t history_length, const boost::shared_ptr<WorldInformation>& initial_world_information)
                           : view_buffer_(history_length*10), last_world_information_(initial_world_information) {
	view_factories_ = view_factories;
	view_buffer_.push_back(create_views(initial_world_information));
}

RobotTypeRobotControl::~RobotTypeRobotControl() {

}

boost::shared_ptr<boost::array<boost::shared_ptr<View>,kRobotTypeCount> > RobotTypeRobotControl::create_views(const boost::shared_ptr<WorldInformation>& world_information) {
	boost::shared_ptr<boost::array<boost::shared_ptr<View>,kRobotTypeCount> > views(new boost::array<boost::shared_ptr<View>,kRobotTypeCount>());
	for(std::size_t i = 0; i < view_factories_.size(); ++i) {
		(*views)[i] = view_factories_[i]->create_new_view_instance(world_information);
	}
	return views;
}

void RobotTypeRobotControl::update(const boost::shared_ptr<WorldInformation>& world_information) {
	view_buffer_.push_back(create_views(world_information));
	last_world_information_ = world_information;
}

void RobotTypeRobotControl::compute_view(Robot& robot) {
	std::size_t index = static_cast<std::size_t>(last_world_information_->get_according_robot_data(robot.id()).type());
	robot.set_view((*view_buffer_.back())[index]);
	last_world_information_->get_according_robot_data(robot.id()).set_view((*view_buffer_.back())[index]);

}

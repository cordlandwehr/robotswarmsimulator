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
 * uniform_robot_control.cc
 *
 *  Created on: 25.02.2009
 *      Author: Daniel
 */

#include <SimulationKernel/uniform_robot_control.h>

#include "../Views/view.h"
#include "../Views/abstract_view_factory.h"
#include "../Model/robot.h"
#include "../Model/world_information.h"

UniformRobotControl::UniformRobotControl(boost::shared_ptr<AbstractViewFactory> view_factory, std::size_t history_length, const boost::shared_ptr<WorldInformation>& initial_world_information)
                           : view_factory_(view_factory), view_buffer_(history_length*10), last_world_information_(initial_world_information) {
	boost::shared_ptr<View> view(view_factory_->create_new_view_instance(initial_world_information));
	view_buffer_.push_back(view);
}

UniformRobotControl::~UniformRobotControl() {

}

void UniformRobotControl::update(const boost::shared_ptr<WorldInformation>& world_information) {
	boost::shared_ptr<View> view(view_factory_->create_new_view_instance(world_information));
	view_buffer_.push_back(view);
	last_world_information_ = world_information;
}

void UniformRobotControl::compute_view(Robot& robot) {
	robot.set_view(view_buffer_.back());
	last_world_information_->get_according_robot_data(robot.id()).set_view((view_buffer_.back()));

}

void UniformRobotControl::set_view_factory(const boost::shared_ptr<AbstractViewFactory>& view_factory) {
	view_factory_ = view_factory;
}

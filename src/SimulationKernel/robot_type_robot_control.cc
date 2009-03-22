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
                           : view_buffer_(history_length), last_world_information_(initial_world_information) {
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

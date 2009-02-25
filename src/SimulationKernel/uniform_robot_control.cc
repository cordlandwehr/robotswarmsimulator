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

UniformRobotControl::UniformRobotControl(boost::shared_ptr<AbstractViewFactory> view_factory, std::size_t history_length, const WorldInformation& initial_world_information)
                           : view_factory_(view_factory), view_buffer_(history_length) {
	boost::shared_ptr<View> view(view_factory_->create_new_view_instance(initial_world_information));
	view_buffer_.push_back(view);
}

UniformRobotControl::~UniformRobotControl() {

}

void UniformRobotControl::update(const WorldInformation& world_information) {
	boost::shared_ptr<View> view(view_factory_->create_new_view_instance(world_information));
	view_buffer_.push_back(view);
}

void UniformRobotControl::compute_view(Robot& robot) {
	robot.set_view(boost::weak_ptr<View>(view_buffer_.back()));
}

void UniformRobotControl::set_view_factory(const boost::shared_ptr<AbstractViewFactory>& view_factory) {
	view_factory_ = view_factory;
}

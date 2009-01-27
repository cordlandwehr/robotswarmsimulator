/*
 * robot_control.cc
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#include "robot_control.h"
#include "../Events/handle_requests_event.h"
#include "../Views/view.h"
#include "../Views/abstract_view_factory.h"

RobotControl::RobotControl(boost::shared_ptr<AbstractViewFactory> view_factory, std::size_t history_length, const WorldInformation& initial_world_information)
                           : view_factory_(view_factory), view_buffer_(history_length) {
	boost::shared_ptr<View> view(view_factory_->create_new_view_instance(initial_world_information));
	view_buffer_.push_back(view);
}

RobotControl::~RobotControl() {

}

void RobotControl::update(const WorldInformation& world_information, boost::shared_ptr<Event> last_event) {
	if(boost::dynamic_pointer_cast<HandleRequestsEvent>(last_event)) {
		boost::shared_ptr<View> view(view_factory_->create_new_view_instance(world_information));
		view_buffer_.push_back(view);
	}
}

std::set<boost::shared_ptr<Request> > RobotControl::compute_new_request(Robot& robot) {
	return robot.compute();
}

void RobotControl::compute_view(Robot& robot) {
	robot.set_view(boost::weak_ptr<View>(view_buffer_.back()));
}

void RobotControl::set_view_facotry(const boost::shared_ptr<AbstractViewFactory>& view_factory) {
	view_factory_ = view_factory;
}

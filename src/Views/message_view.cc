/*
 * message_view.cc
 *
 *  Created on: 10.08.2011
 *      Author: Jonas
 */

#include "message_view.h"

MessageView::MessageView() {
}

MessageView::~MessageView() {
}

boost::shared_ptr<Message> MessageView::get_message(RobotData& robot) {
	boost::shared_ptr<Message> m = robot.get_message();
	if(robot.get_number_of_messages()>0)
		robot.pop_front_message();
	return m;
}

std::size_t MessageView::get_number_of_messages(const RobotData& robot) const {
	return robot.get_number_of_messages();
}

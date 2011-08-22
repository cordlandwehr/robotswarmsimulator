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

boost::shared_ptr<MessageIdentifier> MessageView::get_message(const RobotData& robot, std::size_t index) const {
	boost::shared_ptr<MessageIdentifier> m = robot.get_message(index);
	return m;
}

std::size_t MessageView::get_number_of_messages(const RobotData& robot) const {
	return robot.get_number_of_messages();
}


boost::shared_ptr<RobotIdentifier> MessageView::get_sender(const Message& message) const {
	// TODO check if sender is a visiblie neighbor
	return message.sender();
}

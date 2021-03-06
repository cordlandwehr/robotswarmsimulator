/*
 * send_message_request.h
 *
 *  Created on: 22.07.2011
 *      Author: Jonas
 */

#ifndef SEND_MESSAGE_REQUEST_H_
#define SEND_MESSAGE_REQUEST_H_

#include "request.h"
#include "../Model/message.h"


class SendMessageRequest : public Request {
public:
	SendMessageRequest(Robot & robot, boost::shared_ptr<Message> m) :
	    Request(robot),
	    requested_message_(m) {}

	boost::shared_ptr<Message> requested_message() const { return requested_message_; }

private:
	boost::shared_ptr<Message> requested_message_;
};


#endif /* SEND_MESSAGE_REQUEST_H_ */

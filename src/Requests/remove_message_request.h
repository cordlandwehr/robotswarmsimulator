/*
 * remove_message_request.h
 *
 *  Created on: 22.07.2011
 *      Author: Jonas
 */

#ifndef REMOVE_MESSAGE_REQUEST_H_
#define REMOVE_MESSAGE_REQUEST_H_

#include "request.h"
#include "../Model/message.h"


class RemoveMessageRequest : public Request {
public:
	RemoveMessageRequest(Robot & robot, boost::shared_ptr<MessageIdentifier> m_id) :
	    Request(robot),
	    message_id_(m_id) {}

	boost::shared_ptr<MessageIdentifier> requested_message() const { return message_id_; }

private:
	boost::shared_ptr<MessageIdentifier> message_id_;
};


#endif /* REMOVE_MESSAGE_REQUEST_H_ */

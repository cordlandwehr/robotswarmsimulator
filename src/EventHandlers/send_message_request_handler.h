/*
 * send_message_request_handler.h
 *
 *  Created on: 22.07.2011
 *      Author: Jonas
 *
 *  TODO: add message_request_handler.cc
 */

#ifndef SEND_MESSAGE_REQUEST_HANDLER_H_
#define SEND_MESSAGE_REQUEST_HANDLER_H_

#include "../EventHandlers/request_handler.h"

/**
 * \brief Handles MarkerChangeRequests by changing the specified marker as requested.
 */
class SendMessageRequestHandler : public RequestHandler {
public:
		SendMessageRequestHandler(unsigned int seed, double discard_probability,
	                          const History& history)
	: RequestHandler(seed, discard_probability, history) { }

	virtual ~SendMessageRequestHandler() { }

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* MESSAGE_REQUEST_HANDLER_H_ */

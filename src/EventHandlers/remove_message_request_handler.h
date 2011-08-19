/*
 * remove_message_request_handler.h
 *
 *  Created on: 19.08.2011
 *      Author: Jonas
 */

#ifndef REMOVE_MESSAGE_REQUEST_HANDLER_H_
#define REMOVE_MESSAGE_REQUEST_HANDLER_H_

#include "../EventHandlers/request_handler.h"

/**
 * \brief Handles MarkerChangeRequests by changing the specified marker as requested.
 */
class RemoveMessageRequestHandler : public RequestHandler {
public:
		RemoveMessageRequestHandler(unsigned int seed, double discard_probability,
	                          const History& history)
	: RequestHandler(seed, discard_probability, history) { }

	virtual ~RemoveMessageRequestHandler() { }

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* REMOVE_MESSAGE_REQUEST_HANDLER_H_ */

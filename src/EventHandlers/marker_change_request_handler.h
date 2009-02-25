/*
 * marker_change_request_handler.h
 *
 *  Created on: Feb 25, 2009
 *      Author: peter
 */

#ifndef MARKER_DELETE_REQUEST_HANDLER
#define MARKER_DELETE_REQUEST_HANDLER

#include "request_handler.h"


/**
 * \brief Handles MarkerChangeRequests by changing the specified marker as requested.
 */
class MarkerChangeRequestHandler : public RequestHandler {
public:
	MarkerChangeRequestHandler(unsigned int seed, double discard_probability, const History& history)
	: RequestHandler(seed, discard_probability, history) { }

	virtual ~MarkerChangeRequestHandler() { }

protected:
	virtual void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
	                                     boost::shared_ptr<const Request> request);
};

#endif /* MARKER_CHANGE_REQUEST_HANDLER */

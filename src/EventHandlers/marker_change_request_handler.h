/*
 * marker_change_request_handler.h
 *
 *  Created on: Feb 25, 2009
 *      Author: peter
 */

#ifndef MARKER_DELETE_REQUEST_HANDLER
#define MARKER_DELETE_REQUEST_HANDLER

#include "../EventHandlers/request_handler.h"


/**
 * \brief Handles MarkerChangeRequests by changing the specified marker as requested.
 */
class MarkerChangeRequestHandler : public RequestHandler {
public:
	MarkerChangeRequestHandler(unsigned int seed,
	                           double discard_probability,
	                           const History& history)
	: RequestHandler(seed, discard_probability, history) { }

	virtual ~MarkerChangeRequestHandler() { }

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* MARKER_CHANGE_REQUEST_HANDLER */

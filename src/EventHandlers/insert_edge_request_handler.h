/*
 * insert_edge_request_handler.h
 *
 *  Created on: 22.07.2011
 *      Author: Jonas
 *
 *  TODO: add insert_edge_request_handler.cc
 */

#ifndef INSERT_EDGE_REQUEST_HANDLER_H_
#define INSERT_EDGE_REQUEST_HANDLER_H_

#include "../EventHandlers/request_handler.h"

/**
 * \brief Handles MarkerChangeRequests by changing the specified marker as requested.
 */
class InsertEdgeRequestHandler : public RequestHandler {
public:
		InsertEdgeRequestHandler(unsigned int seed, double discard_probability,
	                          const History& history)
	: RequestHandler(seed, discard_probability, history) { }

	virtual ~InsertEdgeRequestHandler() { }

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* INSERT_EDGE_REQUEST_HANDLER_H_ */

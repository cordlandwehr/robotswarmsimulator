/*
 * marker_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef MARKER_REQUEST_HANDLER_H_
#define MARKER_REQUEST_HANDLER_H_

#include "request_handler.h"

/**
 * Standard marker request handler. Fullfills every marker request.
 */
class MarkerRequestHandler : public RequestHandler {
	friend class EventHandlerFactoryTest;
	friend class LoadMainProjectFileTest;
public:
	MarkerRequestHandler(unsigned int seed,
                         double discard_probability,
                         const History& history) : RequestHandler(seed, discard_probability, history) {}
    virtual ~MarkerRequestHandler() {}

protected:
	virtual void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                         boost::shared_ptr<const Request> request);
};

#endif /* MARKER_REQUEST_HANDLER_H_ */

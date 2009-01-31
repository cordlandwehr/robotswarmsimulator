/*
 * vector_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef VECTOR_REQUEST_HANDLER_H_
#define VECTOR_REQUEST_HANDLER_H_

#include "request_handler.h"

class VectorRequestHandler : public RequestHandler {
public:
	VectorRequestHandler(unsigned int seed,
                         double discard_probability,
                         const History& history) : RequestHandler(seed, discard_probability, history) {}
    virtual ~VectorRequestHandler() {}
private:
	virtual void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                         boost::shared_ptr<const Request> request);
};

#endif /* VECTOR_REQUEST_HANDLER_H_ */

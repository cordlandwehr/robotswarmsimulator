/*
 *  test_marker_request_handler.h
 *
 *  Created on: Dec 29, 2008
 *      Author: peter
 */

#ifndef TEST_MARKER_REQUEST_HANDLER_H_
#define TEST_MARKER_REQUEST_HANDLER_H_

#include "../../EventHandlers/marker_request_handler.h"


/**
 * \brief Simple marker request handler that counts the number of (not discarded) requests handled.
 *
 * Apart from the counting, this request handler behaves exactly as the standard marker request handler.
 */
class TestMarkerRequestHandler : public MarkerRequestHandler {
public:
	TestMarkerRequestHandler(unsigned int seed, double discard_probability, const History& history)
	: MarkerRequestHandler(seed, discard_probability, history), nr_handled_(0) {
	}
	
	unsigned int nr_handled() { return nr_handled_; }
	
protected:
	bool handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
								 boost::shared_ptr<const Request> request) {
		nr_handled_++;
		return MarkerRequestHandler::handle_request_reliable(world_information, request);
	}
	
private:
	unsigned int nr_handled_;
};


#endif /* TEST_MARKER_REQUEST_HANDLER_H_ */

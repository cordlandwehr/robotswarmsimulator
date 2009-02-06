/*
 *  test_vector_request_handler.h
 *
 *  Created on: Dec 29, 2008
 *      Author: peter
 */

#ifndef TEST_VECTOR_REQUEST_HANDLER_H_
#define TEST_VECTOR_REQUEST_HANDLER_H_

#include "../../EventHandlers/vector_request_handler.h"


/**
 * \brief Simple vector request handler that counts the number of (not discarded) requests handled.
 *
 * Apart from the counting, this request handler behaves exactly as the standard vector request handler.
 */
class TestVectorRequestHandler : public VectorRequestHandler {
public:
	TestVectorRequestHandler(unsigned int seed, double discard_probability, const History& history)
	: VectorRequestHandler(seed, discard_probability, history), nr_handled_(0) {
	}
	
	unsigned int nr_handled() { return nr_handled_; }
	
protected:
	void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
								 boost::shared_ptr<const Request> request) {
		VectorRequestHandler::handle_request_reliable(world_information, request);
		nr_handled_++;
	}

private:
	unsigned int nr_handled_;
};

#endif /* TEST_VECTOR_REQUEST_HANDLER_H_ */

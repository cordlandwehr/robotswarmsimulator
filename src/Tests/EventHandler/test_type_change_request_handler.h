/*
 *  test_type_change_request_handler.h
 *
 *  Created on: Dec 29, 2008
 *      Author: peter
 */

#ifndef TEST_TYPE_CHANGE_REQUEST_HANDLER_H_
#define TEST_TYPE_CHANGE_REQUEST_HANDLER_H_

#include "../../EventHandlers/type_change_request_handler.h"


/**
 * \brief Simple type_change request handler that counts the number of (not discarded) requests handled.
 *
 * Apart from the counting, this request handler behaves exactly as the standard type_change request handler.
 */
class TestTypeChangeRequestHandler : public TypeChangeRequestHandler {
public:
	TestTypeChangeRequestHandler(unsigned int seed, double discard_probability, const History& history)
	: TypeChangeRequestHandler(seed, discard_probability, history), nr_handled_(0) {
	}
	
	unsigned int nr_handled() { return nr_handled_; }
	
protected:
	void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
								 boost::shared_ptr<const Request> request) {
		TypeChangeRequestHandler::handle_request_reliable(world_information, request);
		nr_handled_++;
	}
	
private:
	unsigned int nr_handled_;
};


#endif /* TEST_TYPE_CHANGE_REQUEST_HANDLER_H_ */

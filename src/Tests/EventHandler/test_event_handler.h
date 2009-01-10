/*
 * test_event_handler.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef TEST_EVENT_HANDLER_H_
#define TEST_EVENT_HANDLER_H_

#include "../../EventHandlers/event_handler.h"

/**
 * simple event handler that does nothing but to log how many times its handle methods have been
 * called.
 *
 */
class TestEventHandler : public EventHandler {
public:
	TestEventHandler(boost::shared_ptr<History> history) : EventHandler(history),
	                                                       calls_acceleration_request_(0),
	                                                       calls_marker_request_(0),
	                                                       calls_position_request_(0),
	                                                       calls_type_change_request_(0),
	                                                       calls_velocity_request_(0) {}

	int calls_acceleration_request() {return calls_acceleration_request_;}
	int calls_marker_request() {return calls_marker_request_;}
	int calls_position_request() {return calls_position_request_;}
	int calls_type_change_request() {return calls_type_change_request_;}
	int calls_velocity_request() {return calls_velocity_request_;}

private:
	/**
	 * Method for handling acceleration requests
	 */
	void handle_acceleration_request(boost::shared_ptr<AccelerationRequest> acceleration_request) {
		calls_acceleration_request_++;
	};

	/**
	 * Method for handling marker requests
	 */
	void handle_marker_request(boost::shared_ptr<MarkerRequest> marker_request) {
		calls_marker_request_++;
	};

	/**
	 * Method for handling position requests
	 */
	void handle_position_request(boost::shared_ptr<PositionRequest> position_request) {
		calls_position_request_++;
	};

	/**
	 * Method for handling type change requests
	 */
	void handle_type_change_request(boost::shared_ptr<TypeChangeRequest> type_change_request) {
		calls_type_change_request_++;
	};

	/**
	 * Method for handling velocity requests
	 */
	void handle_velocity_request(boost::shared_ptr<VelocityRequest> velocity_request) {
		calls_velocity_request_++;
	};

	int calls_acceleration_request_;
	int calls_marker_request_;
	int calls_position_request_;
	int calls_type_change_request_;
	int calls_velocity_request_;

};


#endif /* TEST_EVENT_HANDLER_H_ */

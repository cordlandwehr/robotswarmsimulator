/*
 * test_event_handler.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef TEST_EVENT_HANDLER_H_
#define TEST_EVENT_HANDLER_H_

#include "../../EventHandlers/event_handler.h"
#include "../../Events/event.h"

/**
 * simple event handler that does nothing but to log how many times its handle methods have been
 * called.
 *
 */
class TestEventHandler : public EventHandler {
public:
	TestEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control) :
	                                                       EventHandler(history, robot_control),
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

protected:
	/**
	 * Method for handling acceleration requests
	 */
	void handle_acceleration_request(boost::shared_ptr<WorldInformation> world_information,
	                                 boost::shared_ptr<const AccelerationRequest> acceleration_request) {
		calls_acceleration_request_++;
	};

	/**
	 * Method for handling marker requests
	 */
	void handle_marker_request(boost::shared_ptr<WorldInformation> world_information,
	                           boost::shared_ptr<const MarkerRequest> marker_request) {
		calls_marker_request_++;
	};

	/**
	 * Method for handling position requests
	 */
	void handle_position_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const PositionRequest> position_request) {
		calls_position_request_++;
	};

	/**
	 * Method for handling type change requests
	 */
	void handle_type_change_request(boost::shared_ptr<WorldInformation> world_information,
	                                boost::shared_ptr<const TypeChangeRequest> type_change_request) {
		calls_type_change_request_++;
	};

	/**
	 * Method for handling velocity requests
	 */
	void handle_velocity_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const VelocityRequest> velocity_request) {
		calls_velocity_request_++;
	};

private:
	int calls_acceleration_request_;
	int calls_marker_request_;
	int calls_position_request_;
	int calls_type_change_request_;
	int calls_velocity_request_;

};

/**
 * a nonexisting event class
 */
class ThePigsCanFlyEvent : public Event {
public:
	explicit ThePigsCanFlyEvent(int time): Event(time) {};
};
#endif /* TEST_EVENT_HANDLER_H_ */

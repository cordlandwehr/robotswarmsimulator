/*
 * exact_acceleration_event_handler.h
 *
 *  Created on: 14.01.2009
 *      Author: peter
 */

#ifndef EXACT_ACCELERATION_EVENT_HANDLER_H_
#define EXACT_ACCELERATION_EVENT_HANDLER_H_


#include <boost/smart_ptr.hpp>

#include "event_handler.h"


// forward declarations
class History;
class RobotControl;
class AccelerationRequest;
class WorldInformation;


/**
 * \brief An event handler supporting only AccelerationRequest.
 */
class ExactAccelerationEventHandler : public EventHandler {
public:
	ExactAccelerationEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

protected:
	/**
	 * \brief Provides default handling of acceleration requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param acceleration_request acceleration request to handle
	 */
	void handle_acceleration_request(boost::shared_ptr<WorldInformation> world_information,
	                                 boost::shared_ptr<const AccelerationRequest> acceleration_request) {
		EventHandler::handle_acceleration_request_default(world_information, acceleration_request);
	}
};

#endif /* EXACT_ACCELERATION_EVENT_HANDLER_H_ */

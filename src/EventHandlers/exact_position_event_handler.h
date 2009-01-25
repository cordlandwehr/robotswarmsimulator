/*
 * position_event_handler.h
 *
 *  Created on: 14.01.2009
 *      Author: peter
 */

#ifndef POSITION_EVENT_HANDLER_H_
#define POSITION_EVENT_HANDLER_H_


#include <boost/smart_ptr.hpp>

#include "event_handler.h"


// forward declarations
class History;
class PositionRequest;
class RobotControl;
class WorldInformation;


/**
 * An event handler supporting only PositionRequest for movement.
 */
class ExactPositionEventHandler : public EventHandler {
public:
	ExactPositionEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

protected:
	/**
	 * \brief Provides default handling of position requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param position_request position request to handle
	 */
	void handle_position_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const PositionRequest> position_request) {
		EventHandler::handle_position_request_default(world_information, position_request);
	}
};

#endif /* POSITION_EVENT_HANDLER_H_ */

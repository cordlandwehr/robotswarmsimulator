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
#include "../Requests/position_request.h"

// forward declarations
class History;
class RobotControl;

/**
 * An event handler supporting only PositionRequest for movement.
 *
 * Besides being able to handle a PositionRequest, this event handler can handle:
 * - MarkerRequest
 * - TypeChangeRequest
 */
// TODO(peter) implement marker request handling
class PositionEventHandler : public EventHandler {
public:
	PositionEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

	void handle_position_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const PositionRequest> position_request);
private:
};

#endif /* POSITION_EVENT_HANDLER_H_ */

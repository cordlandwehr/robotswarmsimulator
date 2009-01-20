/*
 * velocity_event_handler.h
 *
 *  Created on: 14.01.2009
 *      Author: peter
 */

#ifndef VELOCITY_EVENT_HANDLER_H_
#define VELOCITY_EVENT_HANDLER_H_

#include <boost/smart_ptr.hpp>

#include "event_handler.h"

// forward declarations
class History;
class RobotControl;

/**
 * An event handler supporting only VelocityRequest for movement.
 *
 * Besides being able to handle a VelocityRequest, this event handler can handle:
 * - MarkerRequest
 * - TypeChangeRequest
 */
// TODO(peter): implement
class ExactVelocityEventHandler : virtual public EventHandler {
public:
	ExactVelocityEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

private:
};

#endif /* VELOCITY_EVENT_HANDLER_H_ */

/*
 * acceleration_event_handler.h
 *
 *  Created on: 14.01.2009
 *      Author: peter
 */

#ifndef ACCELERATION_EVENT_HANDLER_H_
#define ACCELERATION_EVENT_HANDLER_H_

#include <boost/smart_ptr.hpp>

#include "event_handler.h"

// forward declarations
class History;
class RobotControl;

/**
 * An event handler supporting only AccelerationRequest for movement.
 *
 * Besides being able to handle an AccelerationRequest, this event handler can handle:
 * - MarkerRequest
 * - TypeChangeRequest
 */
// TODO(peter): implement
class AccelerationEventHandler : public EventHandler {
public:
	AccelerationEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

private:
};

#endif /* ACCELERATION_EVENT_HANDLER_H_ */

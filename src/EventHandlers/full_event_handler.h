/*
 * full_event_handler.h
 *
 *  Created on: 14.01.2009
 *      Author: peter
 */

#ifndef FULL_EVENT_HANDLER_H_
#define FULL_EVENT_HANDLER_H_

#include <boost/smart_ptr.hpp>

#include "event_handler.h"

// forward declarations
class History;
class RobotControl;

/**
 * An event handler supporting all (currently provided) requests.
 *
 * Namely, it can handle requests of the following type:
 * - AccelerationRequest
 * - MarkerRequest
 * - PositionRequest
 * - TypeChangeRequest
 * - VelocityRequest
 */
// TODO(peter): implement
class FullEventHandler : public EventHandler {
public:
	FullEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

private:
};

#endif /* FULL_EVENT_HANDLER_H_ */

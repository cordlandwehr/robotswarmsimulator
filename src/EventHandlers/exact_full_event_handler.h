/*
 * exact_full_event_handler.h
 *
 *  Created on: Jan 20, 2009
 *      Author: craupach
 */

#ifndef EXACT_FULL_EVENT_HANDLER_H_
#define EXACT_FULL_EVENT_HANDLER_H_

#include "exact_position_event_handler.h"
#include "exact_marker_event_handler.h"

/**
 * This EventHandler handles the following requests:
 * - PositionRequest: As ExactPositionEventHandler (moving the robot to the exact position requested)
 * - VelocityRequest: As ExactVelocityEventHandler (changing the velocity to the exact velocity requested)
 * - AccelerationRequest: As ExactAccelerationEventHandler (changing the acceleration to the exact acceleration requested)
 * - MarkerRequest: As ExactMarkerEventHandler
 * - TypeChangeRequest: As ExactTypeChangeEventHandler
 */
class ExactFullEventHandler : public ExactPositionEventHandler, public ExactMarkerEventHandler {
public:
	ExactFullEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control), ExactPositionEventHandler(history, robot_control), ExactMarkerEventHandler(history, robot_control)
		{ };
};

#endif /* EXACT_FULL_EVENT_HANDLER_H_ */

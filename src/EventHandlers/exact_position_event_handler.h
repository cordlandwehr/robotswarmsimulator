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
class MarkerRequest;
class PositionRequest;

class WorldInformation;

class History;
class RobotControl;


/**
 * An event handler supporting only PositionRequest for movement.
 */
// TODO(peter) implement correct handling of position requests for robots with a local coordinate system
// TODO(peter) such simple request handling methods can be implemented using static methods
class ExactPositionEventHandler : virtual public EventHandler {
public:
	ExactPositionEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

private:
	void handle_position_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const PositionRequest> position_request);

};

#endif /* POSITION_EVENT_HANDLER_H_ */

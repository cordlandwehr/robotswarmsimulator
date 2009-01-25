/*
 * exact_velocity_event_handler.h
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
class VelocityRequest;
class WorldInformation;


/**
 * An event handler supporting only VelocityRequest for movement.
 */
class ExactVelocityEventHandler : virtual public EventHandler {
public:
	ExactVelocityEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

protected:
	/**
	 * \brief Provides default handling of velocity requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param velocity_request velocity request to handle
	 */
	void handle_velocity_request(boost::shared_ptr<WorldInformation> world_information,
								 boost::shared_ptr<const VelocityRequest> velocity_request);
};

#endif /* VELOCITY_EVENT_HANDLER_H_ */

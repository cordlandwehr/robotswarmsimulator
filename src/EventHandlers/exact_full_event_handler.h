/*
 * exact_full_event_handler.h
 *
 *  Created on: Jan 20, 2009
 *      Author: craupach
 */

#ifndef EXACT_FULL_EVENT_HANDLER_H_
#define EXACT_FULL_EVENT_HANDLER_H_

#include "event_handler.h"


/**
 * This EventHandler handles the following requests:
 * - PositionRequest: moving the robot to the exact position requested
 * - VelocityRequest: changing the velocity to the exact velocity requested
 * - AccelerationRequest: changing the acceleration to the exact acceleration requested
 * - MarkerRequest: fulfills the request
 * - TypeChangeRequest: fulfills the request
 */
class ExactFullEventHandler : public EventHandler {
public:
	ExactFullEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
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

	/**
	 * \brief Provides default handling of marker requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param marker_request marker request to handle
	 */
	void handle_marker_request(boost::shared_ptr<WorldInformation> world_information,
							   boost::shared_ptr<const MarkerRequest> marker_request) {
		EventHandler::handle_marker_request_default(world_information, marker_request);
	}

	/**
	 * \brief Provides default handling of position requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param position_request position request to handle
	 */
	void handle_position_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const PositionRequest> position_request) {
		EventHandler::handle_position_request_default(world_information, position_request);
	}

	/**
	 * \brief Provides default handling of type change requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param type_change_request type change request to handle
	 */
	void handle_type_change_request(boost::shared_ptr<WorldInformation> world_information,
	                                boost::shared_ptr<const TypeChangeRequest> type_change_request) {
		EventHandler::handle_type_change_request_default(world_information, type_change_request);
	}

	/**
	 * \brief Provides default handling of velocity requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param velocity_request velocity request to handle
	 */
	void handle_velocity_request(boost::shared_ptr<WorldInformation> world_information,
								 boost::shared_ptr<const VelocityRequest> velocity_request) {
		EventHandler::handle_velocity_request_default(world_information, velocity_request);
	}
};

#endif /* EXACT_FULL_EVENT_HANDLER_H_ */

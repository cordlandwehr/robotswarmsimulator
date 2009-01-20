/*
 * position_event_handler.cc
 *
 *  Created on: 16.01.2009
 *      Author: peter
 */

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/marker_request.h"
#include "../Requests/position_request.h"

#include "../Utilities/vector3d.h"

#include "exact_position_event_handler.h"

void ExactPositionEventHandler::handle_position_request(boost::shared_ptr<WorldInformation> world_information,
                                                   boost::shared_ptr<const PositionRequest> position_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = position_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	boost::shared_ptr<Vector3d> new_position(new Vector3d(position_request->requested_position()));
	robot_data.set_position(new_position);
}

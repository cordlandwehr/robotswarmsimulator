/*
 * exact_acceleration_event_handler.cc
 *
 *  Created on: 20.01.2009
 *      Author: peter
 */

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/acceleration_request.h"

#include "../Utilities/vector3d.h"
#include "../Utilities/coord_converter.h"

#include "exact_acceleration_event_handler.h"

void ExactAccelerationEventHandler::handle_acceleration_request(boost::shared_ptr<WorldInformation> world_information,
                                                                boost::shared_ptr<const AccelerationRequest> acceleration_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = acceleration_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	const Vector3d& requested_local_acceleration(acceleration_request->requested_acceleration());
	boost::shared_ptr<Vector3d> requested_global_acceleration;
	// TODO(peter) robot_data should provide a more readable method to query wether the robot uses a local coord. system
	if (robot_data.coordinate_system_axis().get<0>()) // there is a local coordinate system for this robot
		requested_global_acceleration.reset(new Vector3d(*CoordConverter::local_to_global(requested_local_acceleration, robot_data.position(),
		                                                                                  robot_data.coordinate_system_axis())));
	else
		requested_global_acceleration.reset(new Vector3d(requested_local_acceleration));
	robot_data.set_acceleration(requested_global_acceleration);
}

/*
 * exact_velocity_event_handler.cc
 *
 *  Created on: 20.01.2009
 *      Author: peter
 */

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/velocity_request.h"

#include "../Utilities/vector3d.h"
#include "../Utilities/coord_converter.h"

#include "exact_velocity_event_handler.h"

void ExactVelocityEventHandler::handle_velocity_request(boost::shared_ptr<WorldInformation> world_information,
                                                        boost::shared_ptr<const VelocityRequest> velocity_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = velocity_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	const Vector3d& requested_local_velocity(velocity_request->requested_velocity());
	boost::shared_ptr<Vector3d> requested_global_velocity;
	// TODO(peter) robot_data should provide a more readable method to query wether the robot uses a local coord. system
	if (robot_data.coordinate_system_axis().get<0>()) // there is a local coordinate system for this robot
		requested_global_velocity.reset(new Vector3d(*CoordConverter::local_to_global(requested_local_velocity, robot_data.coordinate_system_axis())));
	else
		requested_global_velocity.reset(new Vector3d(requested_local_velocity));
	robot_data.set_velocity(requested_global_velocity);
}

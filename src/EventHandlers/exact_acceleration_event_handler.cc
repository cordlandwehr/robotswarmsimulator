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
	// TODO(peter) implement
}

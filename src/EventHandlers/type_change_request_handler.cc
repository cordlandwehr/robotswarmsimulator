/*
 * type_change_request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/type_change_request.h"

#include "type_change_request_handler.h"

bool TypeChangeRequestHandler:: handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                                        boost::shared_ptr<const Request> request) {
	boost::shared_ptr<const TypeChangeRequest> type_change_request =
	     boost::dynamic_pointer_cast<const TypeChangeRequest> (request);
	if(!type_change_request) {
		throw std::invalid_argument("Not a type change request.");
	}

	const boost::shared_ptr<RobotIdentifier>& robot_id = type_change_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	robot_data.set_type(type_change_request->requested_type());
	return true;
}


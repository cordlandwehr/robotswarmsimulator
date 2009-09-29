/*
 * marker_request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#include "../EventHandlers/marker_request_handler.h"

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/marker_request.h"

bool MarkerRequestHandler:: handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {
	boost::shared_ptr<const MarkerRequest> marker_request =
	     boost::dynamic_pointer_cast<const MarkerRequest> (request);
	if(!marker_request) {
		throw std::invalid_argument("Not a marker request.");
	}

	const boost::shared_ptr<RobotIdentifier>& robot_id =
	    marker_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	boost::shared_ptr<MarkerInformation> new_marker_information(
	    new MarkerInformation(marker_request->requested_marker_information()));
	robot_data.set_marker_information(new_marker_information);
	return true;
}

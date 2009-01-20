/*
 * exact_marker_event_handler.cc
 *
 *  Created on: Jan 20, 2009
 *      Author: craupach
 */

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/marker_request.h"

#include "exact_marker_event_handler.h"


void ExactMarkerEventHandler::handle_marker_request(boost::shared_ptr<WorldInformation> world_information,
                                                 boost::shared_ptr<const MarkerRequest> marker_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = marker_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	boost::shared_ptr<MarkerInformation> new_marker_information(marker_request->requested_marker_information().clone());
	robot_data.set_marker_information(new_marker_information);
}


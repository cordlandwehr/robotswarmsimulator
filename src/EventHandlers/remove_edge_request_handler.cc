/*
 * remove_edge_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "remove_edge_request_handler.h"

#include "../Model/robot_data.h"
#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/remove_edge_request.h"

class RobotData;

bool RemoveEdgeRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	boost::shared_ptr<const RemoveEdgeRequest> edge_request =
		    boost::dynamic_pointer_cast<const RemoveEdgeRequest>(request);

	boost::shared_ptr<EdgeIdentifier> e_id = edge_request->requested_edge();
	if(world_information->edge_exists(e_id)){
		boost::shared_ptr<Edge> e = world_information->get_according_edge(e_id);

		if(world_information->robot_exists(e->getRobot1()) && world_information->robot_exists(e->getRobot2())){
			world_information->remove_edge(e);
		}
	}

	return true;
}

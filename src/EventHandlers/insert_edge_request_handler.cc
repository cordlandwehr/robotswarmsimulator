/*
 * insert_edge_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "insert_edge_request_handler.h"

#include "../Model/robot_data.h"
#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/insert_edge_request.h"

class RobotData;

bool InsertEdgeRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	boost::shared_ptr<const InsertEdgeRequest> edge_request =
		    boost::dynamic_pointer_cast<const InsertEdgeRequest>(request);

	boost::shared_ptr<Edge> e = edge_request->requested_edge();

	RobotData& rd1 = world_information->get_according_robot_data(e->getRobot1());
	RobotData& rd2 = world_information->get_according_robot_data(e->getRobot2());

	// add requested edge to world_information and to adjacency list of robots
	rd1.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e->id()));
	rd2.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e->id()));

	world_information->add_edge(e);

	return true;
}

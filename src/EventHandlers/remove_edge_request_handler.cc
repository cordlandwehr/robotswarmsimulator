/*
 * remove_edge_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "remove_edge_request_handler.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/remove_edge_request.h"


bool RemoveEdgeRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	// TODO: remove requested edge from world_information and from adjacency list of robots

	boost::shared_ptr<const RemoveEdgeRequest> edge_request =
		    boost::dynamic_pointer_cast<const RemoveEdgeRequest>(request);
}

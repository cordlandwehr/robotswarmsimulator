/*
 * insert_edge_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "insert_edge_request_handler.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/insert_edge_request.h"


bool InsertEdgeRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	// TODO: add requested edge to world_information and to adjacency list of robots

	boost::shared_ptr<const InsertEdgeRequest> edge_request =
		    boost::dynamic_pointer_cast<const InsertEdgeRequest>(request);

	return true;
}

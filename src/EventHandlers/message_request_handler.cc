/*
 * message_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "message_request_handler.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/message_request.h"


bool MessageRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	// TODO: check if there is an edge/edge exists from sender to addressee

	boost::shared_ptr<const MessageRequest> edge_request =
		    boost::dynamic_pointer_cast<const MessageRequest>(request);
}

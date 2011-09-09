/*
 * remove_message_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "remove_message_request_handler.h"

#include "../Model/robot_data.h"
#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/remove_message_request.h"

class RobotData;

bool RemoveMessageRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	boost::shared_ptr<const RemoveMessageRequest> remove_message_request =
		    boost::dynamic_pointer_cast<const RemoveMessageRequest>(request);

	boost::shared_ptr<MessageIdentifier> m_id = remove_message_request->requested_message();
	boost::shared_ptr<Message> m = world_information->get_according_message(m_id);

	world_information->remove_message(m);
	return true;
}

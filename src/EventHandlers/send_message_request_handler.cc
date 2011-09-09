/*
 * send_message_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "send_message_request_handler.h"

#include "../Model/identifier.h"
#include "../Model/robot_identifier.h"
#include "../Model/message_identifier.h"
#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/send_message_request.h"
#include "../SimulationControl/time_point.h"
#include "../Views/view.h"

#include <boost/foreach.hpp>

class RobotIdentifier;
class MessageIdentifier;
class Identifier;
class TimePoint;


bool SendMessageRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	boost::shared_ptr<const SendMessageRequest> message_request =
		    boost::dynamic_pointer_cast<const SendMessageRequest>(request);

	boost::shared_ptr<Message> m = message_request->requested_message();

	RobotData& rd_sender = world_information->get_according_robot_data(m->sender());
	RobotData& rd_receiver = world_information->get_according_robot_data(m->receiver());

	std::vector<boost::shared_ptr<RobotIdentifier> > neighbors = rd_sender.robot().get_view()->get_visible_robots(rd_sender.robot());

	// check if receiver is a neighbor of the sender
	bool is_neighbor = false;
	BOOST_FOREACH(const boost::shared_ptr<RobotIdentifier>& r, neighbors) {
		if(r->id() == rd_receiver.id()->id()){
			is_neighbor = true;
			break;
		}
	}

	if(is_neighbor){
		// put message in queue
		world_information->add_message(m);
		return true;
	} else {
		return false;
	}
}

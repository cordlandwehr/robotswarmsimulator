/*
 * message_request_handler.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "message_request_handler.h"

#include "../Model/identifier.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/message_request.h"

#include <boost/foreach.hpp>

class RobotIdentifier;
class Identifier;


bool MessageRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {

	boost::shared_ptr<const MessageRequest> message_request =
		    boost::dynamic_pointer_cast<const MessageRequest>(request);

	boost::shared_ptr<Message> m = message_request->requested_message();

	RobotData& rd_sender = world_information->get_according_robot_data(m->sender());
	RobotData& rd_receiver = world_information->get_according_robot_data(m->receiver());

	// check if there exists an edge from sender to receiver
	bool edge_exists = false;
	std::vector<boost::shared_ptr<EdgeIdentifier> > edges = rd_sender.get_edges();
	BOOST_FOREACH(const boost::shared_ptr<EdgeIdentifier>& ei, edges) {
		boost::shared_ptr<Edge> e = world_information->get_according_edge(ei);

		// check if edge is directed or undirected
		if(boost::shared_ptr<DirectedEdge> de = boost::dynamic_pointer_cast<DirectedEdge> (e)){
			if(de->target()->id() == m->receiver()->id()){
				edge_exists = true;
			}
		} else {
			if(e->getRobot1()->id() == m->receiver()->id() || e->getRobot2()->id() == m->receiver()->id()){
				edge_exists = true;
			}
		}
	}

	if(edge_exists){
		// put message in queue
		rd_receiver.push_back_message(m);
		return true;
	} else {
		return false;
	}
}

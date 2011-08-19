/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * event_handler.cc
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#include "../EventHandlers/event_handler.h"

#include <set>
#include <typeinfo>

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>

#include "../Model/world_information.h"
#include "../Model/sphere.h"
#include "../Model/box.h"
#include "../Model/robot_data.h"
#include "../Model/identifier.h"
#include "../Model/robot_identifier.h"

#include "../Events/compute_event.h"
#include "../Events/event.h"
#include "../Events/handle_requests_event.h"
#include "../Events/look_event.h"
#include "../Events/world_modifier_event.h"

#include "../Requests/marker_request.h"
#include "../Requests/position_request.h"
#include "../Requests/request.h"
#include "../Requests/message_request.h"
#include "../Requests/insert_edge_request.h"
#include "../Requests/remove_edge_request.h"

#include "../SimulationControl/history.h"
#include "../SimulationControl/time_point.h"
#include "../SimulationKernel/simulation_listener.h"
#include "../SimulationKernel/robot_control.h"

#include "../Utilities/console_output.h"


using std::vector;
using boost::shared_ptr;

void EventHandler::handle_event(shared_ptr<Event> event,
                                TimePoint& time_point) {
   
	// check that it is not for a past time
	int time_difference = event->time() - time_of_last_event_;
	if(time_difference < 0) {
		// the event is in the past.
		throw std::invalid_argument("Event in the past!");
	}
	time_of_last_event_ = event->time();

	// Try to cast the pointer to look event, compute event, handle_request event
	// to see what kind of event it is.
	// The cast will return NULL if it is the wrong kind of event.
	// A shared_ptr pointing to NULL converts to FALSE in a condition.
	if(shared_ptr<LookEvent> look_event =
	   boost::dynamic_pointer_cast<LookEvent>(event)) {
		time_point.set_world_information(handle_look_event(look_event));
	} else if(shared_ptr<ComputeEvent> compute_event =
	          boost::dynamic_pointer_cast<ComputeEvent>(event)) {
		time_point.set_world_information(handle_compute_event(compute_event));
	} else if(shared_ptr<HandleRequestsEvent> handle_requests_event =
	          boost::dynamic_pointer_cast<HandleRequestsEvent> (event)) {
		time_point.set_world_information(
		    handle_handle_requests_event(handle_requests_event));
	} else if(shared_ptr<WorldModifierEvent> world_modifier_event =
	          boost::dynamic_pointer_cast<WorldModifierEvent> (event)) {
		time_point.set_world_information(
            handle_world_modifier_event(world_modifier_event));
	} else {
		throw std::invalid_argument("Illegal type of event.");
	}
	
	// update all simulation listeners
	update_listeners(time_point, event);
}

shared_ptr<WorldInformation> EventHandler::handle_look_event(
    shared_ptr<LookEvent> look_event) {
	// produce extrapolated world information
	shared_ptr<WorldInformation> new_world_information = 
		extrapolate_old_world_information(look_event->time());

	// update robot control with the extrapolated world information
	robot_control_->update(new_world_information);

	BOOST_FOREACH(shared_ptr<Robot> robot, look_event->robot_subset()) {
		robot_control_->compute_view(*robot);
	}

	return new_world_information;
}

shared_ptr<WorldInformation> EventHandler::handle_compute_event(
    shared_ptr<ComputeEvent> compute_event) {
	BOOST_FOREACH(shared_ptr<Robot> robot, compute_event->robot_subset()) {
		// compute the requests for this robot.
		std::set<shared_ptr<Request> > request_set =
		    robot_control_->compute_new_request(*robot);

		// add the requests to the event.
		BOOST_FOREACH(shared_ptr<Request> request, request_set) {
			compute_event->add_to_requests(request);
		}
	}
	return extrapolate_old_world_information(compute_event->time());
}

shared_ptr<WorldInformation> EventHandler::handle_handle_requests_event(
    shared_ptr<HandleRequestsEvent> handle_requests_event) {
	// produce extrapolated world information
	shared_ptr<WorldInformation> new_world_information =
	    extrapolate_old_world_information(handle_requests_event->time());

	// handle requests
	BOOST_FOREACH(shared_ptr<const Request> request,
	              handle_requests_event->requests()) {
		bool handled_as_expected = true;

		// Try to cast the pointer to all types of request to see what
		// kind of request it is.
		// The cast will return NULL if it is the wrong kind of request.
		// A shared_ptr pointing to NULL converts to FALSE in a condition.
		if(shared_ptr<const PositionRequest> position_request =
		   boost::dynamic_pointer_cast<const PositionRequest> (request)) {
			if(position_request_handler_) {
				handled_as_expected =
				    position_request_handler_->handle_request(
				        new_world_information,
				        position_request);
			} else {
				std::cerr << "No Position Request Handler Set" << std::endl;
			}
		} else if(shared_ptr<const MarkerRequest> marker_request =
		          boost::dynamic_pointer_cast<const MarkerRequest> (request)) {
			if(marker_request_handler_) {
				handled_as_expected =
				    marker_request_handler_->handle_request(
				        new_world_information,
				        marker_request);
			} else {
				std::cerr << "No Marker Request Handler Set" << std::endl;
			}
		} else if(shared_ptr<const MessageRequest> message_request =
				 boost::dynamic_pointer_cast<const MessageRequest>(request)) {
			if(message_request_handler_) {
				handled_as_expected =
						message_request_handler_->handle_request(
				        new_world_information,
				        message_request);
			} else {
				std::cerr << "No Message Request Handler Set" << std::endl;
			}
		} else if(shared_ptr<const InsertEdgeRequest> insert_edge_request =
				 boost::dynamic_pointer_cast<const InsertEdgeRequest>(request)) {
			if(insert_edge_request_handler_) {
				handled_as_expected =
						insert_edge_request_handler_->handle_request(
				        new_world_information,
				        insert_edge_request);
			} else {
				std::cerr << "No Insert Edge Request Handler Set" << std::endl;
			}
		} else if(shared_ptr<const RemoveEdgeRequest> remove_edge_request =
				 boost::dynamic_pointer_cast<const RemoveEdgeRequest>(request)) {
			if(remove_edge_request_handler_) {
				handled_as_expected =
						remove_edge_request_handler_->handle_request(
				        new_world_information,
				        remove_edge_request);
			} else {
				std::cerr << "No Remove Edge Request Handler Set" << std::endl;
			}
		} else {
			throw std::invalid_argument("Illegal type of request.");
		}

		// inform robot data about the success state of its request
		RobotData& robot_data =
		    new_world_information->get_according_robot_data(
		        request->robot().id());
		robot_data.set_last_request_successful(handled_as_expected);
	}

	return new_world_information;
}

shared_ptr<WorldInformation> EventHandler::handle_world_modifier_event(
    shared_ptr<WorldModifierEvent> world_modifier_event) {
	// produce extrapolated world information
	shared_ptr<WorldInformation> new_world_information =
        extrapolate_old_world_information(world_modifier_event->time());
    
    // debug message (annoucing the event)
    ConsoleOutput::log(ConsoleOutput::EventHandler, ConsoleOutput::debug)
        << "Handling WorldModifierEvent (time = "
        << world_modifier_event->time()
        << ").";
    
    // create anonymous handle request event for the generated requests
    shared_ptr<HandleRequestsEvent> handle_request_event
        (new HandleRequestsEvent(world_modifier_event->time()));
    
    BOOST_FOREACH(shared_ptr<WorldModifier> modifier,
                  world_modifier_event->world_modifier_set()) {
        // debug message (modifier id)
        ConsoleOutput::log(ConsoleOutput::EventHandler, ConsoleOutput::debug)
            << "Executing WorldModifier (id = "
            << modifier->get_algorithm_id()
            << ").";
        
        // compute requests
        std::set< shared_ptr<Request> > request_set = modifier->compute(new_world_information);
        
        // add requests to compute event
        // add the requests to the event.
		BOOST_FOREACH(shared_ptr<Request> request, request_set) {
			handle_request_event->add_to_requests(request);
		}
    }
    
    // process anonymous handle request event, return modified world
    //return handle_handle_requests_event(handle_request_event);
    return new_world_information;
}

shared_ptr<WorldInformation> EventHandler::extrapolate_old_world_information(
    int time) {
	// get the old world information to extrapolate from
	const WorldInformation& old_world_information =
	    history_->get_newest().world_information();

	// create new world information
	shared_ptr<WorldInformation> new_world_information(
	    new WorldInformation(old_world_information));

	// set the time
	new_world_information->set_time(time);

// 	// compute the time difference between old and new world_information
// 	int time_difference = time - old_world_information.time();
// 
// 	// extrapolate all robots
// 	BOOST_FOREACH(shared_ptr<RobotData> old_robot,
// 	              old_world_information.robot_data()) {
// 		RobotData &new_robot =
// 		    new_world_information->get_according_robot_data(
// 		        boost::dynamic_pointer_cast<RobotIdentifier> (
// 		            old_robot->id()));
// 		new_robot.set_position(
// 		    old_robot->extrapolated_position(time_difference));
// 		new_robot.set_velocity(
// 		    old_robot->extrapolated_velocity(time_difference));
// 	}

	return new_world_information;
}

void EventHandler::register_listener(shared_ptr<SimulationListener> listener) {
	listeners_.push_back(listener);
}

void EventHandler::update_listeners(TimePoint& time_point,
                                    shared_ptr<Event> event) {
	BOOST_FOREACH(shared_ptr<SimulationListener> listener, listeners_) {
		listener->update(time_point, event);
	}
}

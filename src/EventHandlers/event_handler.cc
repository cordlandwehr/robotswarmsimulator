/*
 * event_handler.cc
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <typeinfo>

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

#include "../Requests/acceleration_request.h"
#include "../Requests/marker_request.h"
#include "../Requests/position_request.h"
#include "../Requests/request.h"
#include "../Requests/type_change_request.h"
#include "../Requests/velocity_request.h"
#include "../Requests/marker_change_request.h"

#include "../SimulationControl/history.h"
#include "../SimulationKernel/simulation_listener.h"
#include "../SimulationKernel/robot_control.h"

#include "event_handler.h"
#include <iostream>

void EventHandler::handle_event(boost::shared_ptr<Event> event) {
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
	if(boost::shared_ptr<LookEvent> look_event = boost::dynamic_pointer_cast<LookEvent>(event)) {
		handle_look_event(look_event);
	} else if(boost::shared_ptr<ComputeEvent> compute_event = boost::dynamic_pointer_cast<ComputeEvent>(event)) {
		handle_compute_event(compute_event);
	} else if(boost::shared_ptr<HandleRequestsEvent> handle_requests_event =
	          boost::dynamic_pointer_cast<HandleRequestsEvent> (event)) {
		handle_handle_requests_event(handle_requests_event);
	} else {
		throw std::invalid_argument("Illegal type of event.");
	}

	// update all simulation listeners
	update_listeners(event);
}

void EventHandler::handle_look_event(boost::shared_ptr<LookEvent> look_event) {
	// produce extrapolated world information
	boost::shared_ptr<WorldInformation> new_world_information =
		extrapolate_old_world_information(look_event->time());

	// push back new world information
	history_->insert(new_world_information);

	// update robot control with the extrapolated world information
	robot_control_->update(*new_world_information);

	BOOST_FOREACH(boost::shared_ptr<Robot> robot, look_event->robot_subset()) {
		robot_control_->compute_view(*robot);
	}
}

void EventHandler::handle_compute_event(boost::shared_ptr<ComputeEvent> compute_event) {
  BOOST_FOREACH(boost::shared_ptr<Robot> robot, compute_event->robot_subset()) {
	  // compute the requests for this robot.
	  std::set<boost::shared_ptr<Request> > request_set = robot_control_->compute_new_request(*robot);

	  // add the requests to the event.
	  BOOST_FOREACH(boost::shared_ptr<Request> request, request_set) {
		  compute_event->add_to_requests(request);
	  }
  }
}

void EventHandler::handle_handle_requests_event(boost::shared_ptr<HandleRequestsEvent> handle_requests_event) {
	// produce extrapolated world information
	boost::shared_ptr<WorldInformation> new_world_information =
	    extrapolate_old_world_information(handle_requests_event->time());

	// handle requests
	BOOST_FOREACH(boost::shared_ptr<const Request> request, handle_requests_event->requests()) {

		// Try to cast the pointer to all types of request to see what kind of request it is.
		// The cast will return NULL if it is the wrong kind of request.
		// A shared_ptr pointing to NULL converts to FALSE in a condition.
		if(boost::shared_ptr<const PositionRequest> position_request =
		   boost::dynamic_pointer_cast<const PositionRequest> (request)) {
			if(position_request_handler_) {
				position_request_handler_->handle_request(new_world_information, position_request);
			} else {
				std::cerr << "No Position Request Handler Set" << std::endl;
			}
		} else if(boost::shared_ptr<const AccelerationRequest> acceleration_request =
		          boost::dynamic_pointer_cast<const AccelerationRequest> (request)) {
			if(acceleration_request_handler_) {
				acceleration_request_handler_->handle_request(new_world_information, acceleration_request);
			} else {
				std::cerr << "No Acceleration Request Handler Set" << std::endl;
			}
		} else if(boost::shared_ptr<const MarkerRequest> marker_request =
		          boost::dynamic_pointer_cast<const MarkerRequest> (request)) {
			if(marker_request_handler_) {
				marker_request_handler_->handle_request(new_world_information, marker_request);
			} else {
				std::cerr << "No Marker Request Handler Set" << std::endl;
			}
		} else if(boost::shared_ptr<const TypeChangeRequest> type_change_request =
		          boost::dynamic_pointer_cast<const TypeChangeRequest>(request)) {
			if(type_change_request_handler_) {
				type_change_request_handler_->handle_request(new_world_information, type_change_request);
			} else {
				std::cerr << "No Type Change Request Handler Set" << std::endl;
			}
		} else if(boost::shared_ptr<const VelocityRequest> velocity_request =
		          boost::dynamic_pointer_cast<const VelocityRequest> (request)) {
			if(velocity_request_handler_) {
				velocity_request_handler_->handle_request(new_world_information, velocity_request);
			} else {
				std::cerr << "No Velocity Request Handler Set" << std::endl;
			}
		} else if(boost::shared_ptr<const MarkerChangeRequest> marker_change_request =
		          boost::dynamic_pointer_cast<const MarkerChangeRequest> (request)) {
			if (marker_change_request_handler_) {
				marker_change_request_handler_->handle_request(new_world_information, marker_change_request);
			} else {
				std::cerr << "No Marker Change Request Handler Set" << std::endl;
			}
		} else {
			throw std::invalid_argument("Illegal type of request.");
		}
	}

	// push back new world information
	history_->insert(new_world_information);
}

boost::shared_ptr<WorldInformation> EventHandler::extrapolate_old_world_information(int time) {
	// get the old world information to extrapolate from
	const WorldInformation& old_world_information = history_->get_newest();

	// create new world information
	boost::shared_ptr<WorldInformation> new_world_information(new WorldInformation(old_world_information));

	// set the time
	new_world_information->set_time(time);

	// compute the time difference between old and new world_information
	int time_difference = time - old_world_information.time();

	// extrapolate all robots
	BOOST_FOREACH(boost::shared_ptr<RobotData> old_robot, old_world_information.robot_data()) {
		RobotData &new_robot =
		    new_world_information->get_according_robot_data(boost::dynamic_pointer_cast<RobotIdentifier> (old_robot->id()));
		new_robot.set_position(old_robot->extrapolated_position(time_difference));
		new_robot.set_velocity(old_robot->extrapolated_velocity(time_difference));
	}

	return new_world_information;
}

void EventHandler::register_listener(boost::shared_ptr<SimulationListener> listener) {
	listeners_.push_back(listener);
}

void EventHandler::update_listeners(boost::shared_ptr<Event> event) {
	BOOST_FOREACH(boost::shared_ptr<SimulationListener> listener, listeners_) {
		listener->update(history_->get_newest(), event);
	}
}

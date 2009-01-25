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

#include "../SimulationControl/history.h"
#include "../SimulationKernel/simulation_listener.h"
#include "../SimulationKernel/robot_control.h"

#include "../Utilities/coord_converter.h"

#include "event_handler.h"
#include <iostream>

EventHandler::~EventHandler() {}

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
		// The cast will return NULL if it is the wrong kind of event.
		// A shared_ptr pointing to NULL converts to FALSE in a condition.
		if(boost::shared_ptr<const PositionRequest> position_request =
		   boost::dynamic_pointer_cast<const PositionRequest> (request)) {
			handle_position_request(new_world_information, position_request);

		} else if(boost::shared_ptr<const AccelerationRequest> acceleration_request =
		          boost::dynamic_pointer_cast<const AccelerationRequest> (request)) {
			handle_acceleration_request(new_world_information, acceleration_request);

		} else if(boost::shared_ptr<const MarkerRequest> marker_request =
		          boost::dynamic_pointer_cast<const MarkerRequest> (request)) {
			handle_marker_request(new_world_information, marker_request);

		} else if(boost::shared_ptr<const TypeChangeRequest> type_change_request =
		          boost::dynamic_pointer_cast<const TypeChangeRequest>(request)) {
			handle_type_change_request(new_world_information, type_change_request);

		} else if(boost::shared_ptr<const VelocityRequest> velocity_request =
		          boost::dynamic_pointer_cast<const VelocityRequest> (request)) {
			handle_velocity_request(new_world_information, velocity_request);

		} else {
			throw std::invalid_argument("Illegal type of request.");
		}
	}

	// push back new world information
	history_->insert(new_world_information);
}

boost::shared_ptr<WorldInformation> EventHandler::extrapolate_old_world_information(int time) {
	// create new world information
	boost::shared_ptr<WorldInformation> new_world_information(new WorldInformation());

	// get the old world information to extrapolate from
	WorldInformation old_world_information = history_->get_newest();

	// set the time
	new_world_information->set_time(time);

	// compute the time difference between old and new world_infrmation
	int time_difference = time - old_world_information.time();

	// extrapolate all robots
	BOOST_FOREACH(boost::shared_ptr<RobotData> old_robot, old_world_information.robot_data()) {
		// TODO(craupach) this unnecessarily copies position and velocity. May be too inefficient.
		boost::shared_ptr<RobotData> new_robot(new RobotData(*old_robot));
		new_robot->set_position(old_robot->extrapolated_position(time_difference));
		new_robot->set_velocity(old_robot->extrapolated_velocity(time_difference));

		// insert new robot
		// TODO(craupach) test if this preserves the ordering. There should be a way of inserting at a specific
		// position in the array
		new_world_information->add_robot_data(new_robot);
	}

	// copy all obstacles
	BOOST_FOREACH(boost::shared_ptr<Obstacle> old_obstacle, old_world_information.obstacles()) {
		boost::shared_ptr<Obstacle> new_obstacle = boost::dynamic_pointer_cast<Obstacle>(old_obstacle->clone());
		new_world_information->add_obstacle(new_obstacle);
	}

	// copy all markers
	BOOST_FOREACH(boost::shared_ptr<WorldObject> old_marker, old_world_information.markers()) {
		boost::shared_ptr<WorldObject> new_marker(new WorldObject(*old_marker));
		new_world_information->add_marker(new_marker);
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


void EventHandler::handle_acceleration_request_default(boost::shared_ptr<WorldInformation> world_information,
	                                                   boost::shared_ptr<const AccelerationRequest> acceleration_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = acceleration_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	const Vector3d& requested_local_acceleration(acceleration_request->requested_acceleration());
	boost::shared_ptr<Vector3d> requested_global_acceleration;
	// TODO(peter) robot_data should provide a more readable method to query wether the robot uses a local coord. system
	if (robot_data.coordinate_system_axis().get<0>()) // there is a local coordinate system for this robot
		requested_global_acceleration.reset(new Vector3d(*CoordConverter::local_to_global(requested_local_acceleration, robot_data.coordinate_system_axis())));
	else
		requested_global_acceleration.reset(new Vector3d(requested_local_acceleration));
	robot_data.set_acceleration(requested_global_acceleration);
}


void EventHandler::handle_marker_request_default(boost::shared_ptr<WorldInformation> world_information,
		                                         boost::shared_ptr<const MarkerRequest> marker_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = marker_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	boost::shared_ptr<MarkerInformation> new_marker_information(marker_request->requested_marker_information().clone());
	robot_data.set_marker_information(new_marker_information);
}


void EventHandler::handle_position_request_default(boost::shared_ptr<WorldInformation> world_information,
		                                           boost::shared_ptr<const PositionRequest> position_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = position_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	const Vector3d& requested_local_position(position_request->requested_position());
	boost::shared_ptr<Vector3d> requested_global_position;
	// TODO(peter) robot_data should provide a more readable method to query wether the robot uses a local coord. system
	// TODO(craupach) Until now I assumed that every robot would have a coordinate_system set. (Maybe the same for all robots)
	if (robot_data.coordinate_system_axis().get<0>()) // there is a local coordinate system for this robot
		requested_global_position.reset(new Vector3d(*CoordConverter::local_to_global(requested_local_position, robot_data.coordinate_system_axis())));
	else
		requested_global_position.reset(new Vector3d(requested_local_position));
	robot_data.set_position(requested_global_position);
}


void EventHandler::handle_type_change_request_default(boost::shared_ptr<WorldInformation> world_information,
		                                              boost::shared_ptr<const TypeChangeRequest> type_change_request) {
	// TODO(peter) implement
}


void EventHandler::handle_velocity_request_default(boost::shared_ptr<WorldInformation> world_information,
		                                           boost::shared_ptr<const VelocityRequest> velocity_request) {
	const boost::shared_ptr<RobotIdentifier>& robot_id = velocity_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	const Vector3d& requested_local_velocity(velocity_request->requested_velocity());
	boost::shared_ptr<Vector3d> requested_global_velocity;
	// TODO(peter) robot_data should provide a more readable method to query wether the robot uses a local coord. system
	if (robot_data.coordinate_system_axis().get<0>()) // there is a local coordinate system for this robot
		requested_global_velocity.reset(new Vector3d(*CoordConverter::local_to_global(requested_local_velocity, robot_data.coordinate_system_axis())));
	else
		requested_global_velocity.reset(new Vector3d(requested_local_velocity));
	robot_data.set_velocity(requested_global_velocity);
}

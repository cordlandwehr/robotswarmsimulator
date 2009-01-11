/*
 * event_handler.cc
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>

#include "../Model/world_information.h"
#include "../Model/sphere.h"
#include "../Model/box.h"
#include "../Model/robot_data.h"

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

#include "event_handler.h"

#include <iostream>

void EventHandler::handle_event(boost::shared_ptr<Event> event) {
	// check if it is a look event
	boost::shared_ptr<LookEvent> look_event = boost::dynamic_pointer_cast<LookEvent>(event);
	if(look_event.get() != NULL) {
		// TODO(craupach) can this be done more elegantly?
		handle_look_event(look_event);
	} else {
		// check if it is a compute event
		boost::shared_ptr<ComputeEvent> compute_event = boost::dynamic_pointer_cast<ComputeEvent>(event);
		if(compute_event.get() != NULL) {
			handle_compute_event(compute_event);
		} else {
			// check if it is a handle request event
			boost::shared_ptr<HandleRequestsEvent> handle_requests_event =
			     boost::dynamic_pointer_cast<HandleRequestsEvent> (event);
			if(handle_requests_event.get() != NULL) {
				handle_handle_requests_event(handle_requests_event);
			} else {
				// TODO(craupach) should never happen. Throw exception.
			}
		}
	}
	// update all simulation listeners
	update_listeners(event);
}

void EventHandler::handle_look_event(boost::shared_ptr<LookEvent> look_event) {
  // TODO(craupach) to be implementend later after the RobotControl is finished
}

void EventHandler::handle_compute_event(boost::shared_ptr<ComputeEvent> compute_event) {
  // TODO(craupach) to be implemented later after the RobotControl is finished
}

void EventHandler::handle_handle_requests_event(boost::shared_ptr<HandleRequestsEvent> handle_requests_event) {
	// produce extrapolated world information
	boost::shared_ptr<WorldInformation> new_world_information =
	    extrapolate_old_world_information(handle_requests_event->time());

	// handle requests
	BOOST_FOREACH(boost::shared_ptr<Request> request, handle_requests_event->requests()) {

		boost::shared_ptr<PositionRequest> position_request =
		    boost::dynamic_pointer_cast<PositionRequest> (request);
		if(position_request.get() != NULL) {
			handle_position_request(new_world_information, position_request);
		} else {
			boost::shared_ptr<AccelerationRequest> acceleration_request =
			    boost::dynamic_pointer_cast<AccelerationRequest> (request);
			if(acceleration_request.get() != NULL) {
				handle_acceleration_request(new_world_information, acceleration_request);
			} else {
				boost::shared_ptr<MarkerRequest> marker_request =
					boost::dynamic_pointer_cast<MarkerRequest> (request);
				if(marker_request.get() != NULL) {
					handle_marker_request(new_world_information, marker_request);
				} else {
					boost::shared_ptr<TypeChangeRequest> type_change_request =
					    boost::dynamic_pointer_cast<TypeChangeRequest>(request);
					if(type_change_request.get() != NULL) {
						handle_type_change_request(new_world_information, type_change_request);
					} else {
						boost::shared_ptr<VelocityRequest> velocity_request =
						    boost::dynamic_pointer_cast<VelocityRequest> (request);
						if(velocity_request.get() != NULL) {
							handle_velocity_request(new_world_information, velocity_request);
						} else {
							// TODO(craupach) should never really happen. Throw exception.
						}
					}
				}
			}
		}
	}

	// push back new world information
	history_->push_back(new_world_information);
}

boost::shared_ptr<WorldInformation> EventHandler::extrapolate_old_world_information(int time) {
	// create new world information
	boost::shared_ptr<WorldInformation> new_world_information(new WorldInformation());

	// get the old world information to extrapolate from
	WorldInformation old_world_information = history_->get_newest();

	// set the time
	new_world_information->set_time(time);

	// extrapolate all robots
	BOOST_FOREACH(boost::shared_ptr<RobotData> old_robot, old_world_information.robot_data()) {
		// create new robot
		boost::shared_ptr<RobotData> new_robot(new RobotData(old_robot->robot()));

		// TODO(craupach) extrapolate from old one instead of just making a copy.
		// TODO(craupach) there should be some way to copy the identifier too. Ask Martina about how the Kernel team wants to do this.
		// TODO(craupach) the pointer to the robot also needs to be copied.
		// TODO(craupach) marker information needs to be copied.
		boost::shared_ptr<Vector3d> new_acceleration(new Vector3d(old_robot->acceleration()));
		new_robot->set_acceleration(new_acceleration);

		boost::shared_ptr<Vector3d> new_velocity(new Vector3d(old_robot->velocity()));
		new_robot->set_velocity(new_velocity);

		boost::shared_ptr<Vector3d> new_position(new Vector3d(old_robot->position()));
		new_robot->set_position(new_position);

		new_robot->set_status(old_robot->status());
		new_robot->set_type(old_robot->type());

		// insert new robot
		// TODO(craupach) test if this preserves the ordering. There should be a way of inserting at a specific
		// position in the array
		new_world_information->add_robot_data(new_robot);
	}

	// extrapolate all obstacles
	BOOST_FOREACH(boost::shared_ptr<Obstacle> old_obstacle, old_world_information.obstacles()) {
			//TODO(craupach) MarkerPosition needs to be copied
			// copy position
			boost::shared_ptr<Vector3d> new_position(new Vector3d(old_obstacle->position()));

			// check if box or sphere
			boost::shared_ptr<Sphere> old_sphere = boost::dynamic_pointer_cast<Sphere> (old_obstacle);
			if(old_sphere.get() != NULL) {
				boost::shared_ptr<Sphere> new_sphere(new Sphere());
				new_sphere->set_position(new_position);
				new_sphere->set_radius(old_sphere->radius());
				new_world_information->add_obstacle(new_sphere);
			} else {
				boost::shared_ptr<Box> old_box = boost::dynamic_pointer_cast<Box> (old_obstacle);
				if(old_box.get() != NULL) {
					boost::shared_ptr<Box> new_box(new Box());
					new_box->set_position(new_position);
					new_box->set_depth(old_box->depth());
					new_box->set_height(old_box->height());
					new_box->set_width(old_box->width());
					new_world_information->add_obstacle(new_box);
				} else {
					// TODO(craupach) should never happen. Throw exception
				}
			}
	}
	// TODO(craupach) implement this

	// extrapolate all markers
	// TODO(craupach) implement this

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

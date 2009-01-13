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

#include "event_handler.h"


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
				throw std::invalid_argument("Illegal type of world_event.");
			}
		}
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

		boost::shared_ptr<const PositionRequest> position_request =
		    boost::dynamic_pointer_cast<const PositionRequest> (request);
		if(position_request.get() != NULL) {
			handle_position_request(new_world_information, position_request);
		} else {
			boost::shared_ptr<const AccelerationRequest> acceleration_request =
			    boost::dynamic_pointer_cast<const AccelerationRequest> (request);
			if(acceleration_request.get() != NULL) {
				handle_acceleration_request(new_world_information, acceleration_request);
			} else {
				boost::shared_ptr<const MarkerRequest> marker_request =
					boost::dynamic_pointer_cast<const MarkerRequest> (request);
				if(marker_request.get() != NULL) {
					handle_marker_request(new_world_information, marker_request);
				} else {
					boost::shared_ptr<const TypeChangeRequest> type_change_request =
					    boost::dynamic_pointer_cast<const TypeChangeRequest>(request);
					if(type_change_request.get() != NULL) {
						handle_type_change_request(new_world_information, type_change_request);
					} else {
						boost::shared_ptr<const VelocityRequest> velocity_request =
						    boost::dynamic_pointer_cast<const VelocityRequest> (request);
						if(velocity_request.get() != NULL) {
							handle_velocity_request(new_world_information, velocity_request);
						} else {
							throw std::invalid_argument("Illegal type of request.");
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
		boost::shared_ptr<RobotData> new_robot(new RobotData(old_robot->id()->clone(), old_robot->robot()));

		// TODO(craupach) extrapolate from old one instead of just making a copy.

		boost::shared_ptr<Vector3d> new_acceleration(new Vector3d(old_robot->acceleration()));
		new_robot->set_acceleration(new_acceleration);

		boost::shared_ptr<Vector3d> new_velocity(new Vector3d(old_robot->velocity()));
		new_robot->set_velocity(new_velocity);

		boost::shared_ptr<Vector3d> new_position(new Vector3d(old_robot->position()));
		new_robot->set_position(new_position);

		boost::shared_ptr<MarkerInformation> new_marker_information(
		    new MarkerInformation(old_robot->marker_information()));
		new_robot->set_marker_information(new_marker_information);

		new_robot->set_status(old_robot->status());
		new_robot->set_type(old_robot->type());

		// insert new robot
		// TODO(craupach) test if this preserves the ordering. There should be a way of inserting at a specific
		// position in the array
		new_world_information->add_robot_data(new_robot);
	}

	// extrapolate all obstacles
	BOOST_FOREACH(boost::shared_ptr<Obstacle> old_obstacle, old_world_information.obstacles()) {
			// copy position, marker information
			boost::shared_ptr<Vector3d> new_position(new Vector3d(old_obstacle->position()));
			boost::shared_ptr<MarkerInformation> new_marker_information(
			    new MarkerInformation(old_obstacle->marker_information()));

			// check if box or sphere
			boost::shared_ptr<Sphere> old_sphere = boost::dynamic_pointer_cast<Sphere> (old_obstacle);
			if(old_sphere.get() != NULL) {
				boost::shared_ptr<Sphere> new_sphere(new Sphere(old_sphere->id()->clone()));
				new_sphere->set_position(new_position);
				new_sphere->set_marker_information(new_marker_information);
				new_sphere->set_radius(old_sphere->radius());
				new_world_information->add_obstacle(new_sphere);
			} else {
				boost::shared_ptr<Box> old_box = boost::dynamic_pointer_cast<Box> (old_obstacle);
				if(old_box.get() != NULL) {
					boost::shared_ptr<Box> new_box(new Box(old_box->id()->clone()));
					new_box->set_position(new_position);
					new_box->set_marker_information(new_marker_information);
					new_box->set_depth(old_box->depth());
					new_box->set_height(old_box->height());
					new_box->set_width(old_box->width());
					new_world_information->add_obstacle(new_box);
				} else {
					throw std::invalid_argument("Illegal type of world_object.");
				}
			}
	}

	// extrapolate all markers
	BOOST_FOREACH(boost::shared_ptr<WorldObject> old_marker, old_world_information.markers()) {
		boost::shared_ptr<WorldObject> new_marker(new WorldObject(old_marker->id()->clone()));

		boost::shared_ptr<Vector3d> new_position(new Vector3d(old_marker->position()));
		new_marker->set_position(new_position);

		boost::shared_ptr<MarkerInformation> new_marker_information(
		    new MarkerInformation(old_marker->marker_information()));
		new_marker->set_marker_information(new_marker_information);

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

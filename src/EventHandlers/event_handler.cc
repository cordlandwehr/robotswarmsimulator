/*
 * event_handler.cc
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>

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
				// TODO(craupach) should never happen. Log error.
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

}

void EventHandler::register_listener(boost::shared_ptr<SimulationListener> listener) {
	listeners_.push_back(listener);
}

void EventHandler::update_listeners(boost::shared_ptr<Event> event) {
	BOOST_FOREACH(boost::shared_ptr<SimulationListener> listener, listeners_) {
		listener->update(history_->get_newest(), event);
	}
}

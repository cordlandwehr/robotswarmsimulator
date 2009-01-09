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

void EventHandler::handle_event(boost::shared_ptr<Event> event) {
	// check if it is a look event
	LookEvent* look_event = dynamic_cast<LookEvent*> (event.get());
	if(look_event != NULL) {
		// TODO(craupach) can this be done more elegantly?
		boost::shared_ptr<LookEvent> look_event_ptr;
		look_event_ptr.reset(look_event);
		handle_look_event(look_event_ptr);
	} else {
		// check if it is a compute event
		ComputeEvent* compute_event = dynamic_cast<ComputeEvent*> (event.get());
		if(compute_event != NULL) {
			boost::shared_ptr<ComputeEvent> compute_event_ptr;
			compute_event_ptr.reset(compute_event);
			handle_compute_event(compute_event_ptr);
		} else {
			// check if it is a handle request event
			HandleRequestsEvent* handle_requests_event = dynamic_cast<HandleRequestsEvent*> (event.get());
			if(handle_requests_event != NULL) {
				boost::shared_ptr<HandleRequestsEvent> handle_requests_event_ptr;
				handle_requests_event_ptr.reset(handle_requests_event);
				handle_handle_requests_event(handle_requests_event_ptr);
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

void EventHandler::update_listeners(boost::shared_ptr<Event> event) {
	BOOST_FOREACH(boost::shared_ptr<SimulationListener> listener, listeners_) {
		listener->update(history_->get_newest(), event);
	}
}

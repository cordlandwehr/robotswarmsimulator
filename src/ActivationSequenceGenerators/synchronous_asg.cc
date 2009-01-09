/*
 * synchronous_asg.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef SYNCHRONOUS_ASG_CC_
#define SYNCHRONOUS_ASG_CC_

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>

#include "../Events/event.h"
#include "../Events/look_event.h"
#include "../Events/compute_event.h"
#include "../Events/handle_requests_event.h"
#include "../SimulationControl/history.h"

#include "synchronous_asg.h"

void SynchronousASG::initialize(boost::shared_ptr<History> history) {
	// TODO(craupach) this needs to somehow extract a pointer to the set of all robots
}

boost::shared_ptr<Event> SynchronousASG::get_next_event() {
	boost::shared_ptr<Event> event;
	if(time_of_next_event_ % 3 == 0) {
		LookEvent * look_event = new LookEvent(time_of_next_event_);
		event.reset(look_event);
		//TODO(craupach) this needs to add all robots to the event

	} else if(time_of_next_event_ % 3 == 1) {
		ComputeEvent * compute_event = new ComputeEvent(time_of_next_event_);
		event.reset(compute_event);
		//TODO(craupach) again this needs to add all robots to the event

	} else if(time_of_next_event_ % 3 == 2) {
		HandleRequestsEvent * handle_requests_event = new HandleRequestsEvent(time_of_next_event_);
		event.reset(handle_requests_event);
		// copy over all requests
		BOOST_FOREACH(boost::shared_ptr<Request> cur_request, unhandled_request_set_) {
			handle_requests_event->add_to_requests(cur_request);
		}
		unhandled_request_set_.clear();
	}
	time_of_next_event_++;
	return event;
}

void SynchronousASG::update(const WorldInformation& world_information,
                            boost::shared_ptr<Event> last_event) {
	// check if it is a compute event
	ComputeEvent* compute_event = dynamic_cast<ComputeEvent*> (last_event.get());
	if(compute_event != NULL) {
		BOOST_FOREACH(boost::shared_ptr<Request> cur_request, compute_event->requests()) {
			unhandled_request_set_.push_back(cur_request);
		}
	}
}

#endif /* SYNCHRONOUS_ASG_CC_ */

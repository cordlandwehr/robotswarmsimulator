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
#include "../Model/robot_data.h"

#include "synchronous_asg.h"

using std::vector;


void SynchronousASG::initialize(
	    const History& history,
	    const std::vector<boost::shared_ptr<Robot> >& robots,
	    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers){
	// extract robots from robot data
	BOOST_FOREACH(boost::shared_ptr<Robot> robot, robots) {
		robots_.push_back(robot);
	}
}

boost::shared_ptr<Event> SynchronousASG::get_next_event() {
	boost::shared_ptr<Event> event;
	if(time_of_next_event_ % kNumberOfEvents == kTimeToLook) {
		LookEvent * look_event = new LookEvent(time_of_next_event_);
		event.reset(look_event);

		// add all robots. This uses the raw pointer because it is of the right type.
		// (it is protected by a shared_ptr and I'm saving a cast)
		BOOST_FOREACH(boost::shared_ptr<Robot> robot, robots_) {
			look_event->add_to_robot_subset(robot);
		}


	} else if(time_of_next_event_ % kNumberOfEvents == kTimeToCompute) {
		ComputeEvent * compute_event = new ComputeEvent(time_of_next_event_);
		event.reset(compute_event);

		// add all robots. This uses the raw pointer because it is of the right type.
		// (it is protected by a shared_ptr and I'm saving a cast)
		BOOST_FOREACH(boost::shared_ptr<Robot> robot, robots_) {
			compute_event->add_to_robot_subset(robot);
		}

	} else if(time_of_next_event_ % kNumberOfEvents == kTimeToMove) {
		HandleRequestsEvent * handle_requests_event =
		    new HandleRequestsEvent(time_of_next_event_);
		event.reset(handle_requests_event);
		// copy over all requests
		BOOST_FOREACH(boost::shared_ptr<const Request> cur_request,
		              unhandled_request_set_) {
			handle_requests_event->add_to_requests(cur_request);
		}
		unhandled_request_set_.clear();
	}
	time_of_next_event_++;
	return event;
}

void SynchronousASG::update(TimePoint& time_point,
                            boost::shared_ptr<Event> last_event) {
	// check if it is a compute event
	ComputeEvent* compute_event = dynamic_cast<ComputeEvent*> (last_event.get());
	if(compute_event != NULL) {
		BOOST_FOREACH(boost::shared_ptr<const Request> cur_request,
		              compute_event->requests()) {
			unhandled_request_set_.push_back(cur_request);
		}
	}
}

#endif /* SYNCHRONOUS_ASG_CC_ */

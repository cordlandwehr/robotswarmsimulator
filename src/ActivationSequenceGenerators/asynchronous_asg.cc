/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
 * asynchronous_asg.cc
 *
 *  Created on: Jan 24, 2009
 *      Author: craupach
 */

#ifndef ASYNCHRONOUS_ASG_CC_
#define ASYNCHRONOUS_ASG_CC_

#include "asynchronous_asg.h"

#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "../Events/event.h"
#include "../Events/look_event.h"
#include "../Events/compute_event.h"
#include "../Events/handle_requests_event.h"
#include "../SimulationControl/history.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Utilities/distribution_generator.h"

using std::list;
using std::vector;


AsynchronousASG::AsynchronousASG(
    unsigned int seed,
    double participation_probability,
    double p): ActivationSequenceGenerator(),
               time_of_next_event_(0),
               distribution_generator_(new DistributionGenerator(seed)) {
	// initalize the source of randomness
    // needed for time of next event
	distribution_generator_->init_geometric(p);
	// needed to choose robots participating in an event
	distribution_generator_->init_bernoulli(participation_probability);
	// needed to choose type of events
	distribution_generator_->init_uniform(1, 6);
}


void AsynchronousASG::initialize(
	    const History& history,
	    const std::vector<boost::shared_ptr<Robot> >& robots,
	    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers){
	// extract robots from robot data. At start each will need a look event.
	BOOST_FOREACH(boost::shared_ptr<Robot> robot, robots) {
		looking_robots_.push_back(robot);
	}
}

boost::shared_ptr<Event> AsynchronousASG::get_next_event() {
	// 1. Choose which kind of event will be next
	// and constructs an event of that type.
	boost::shared_ptr<Event> event = choose_event_type();

	// 2. Select the robots participating in the event.
	// Note that the event may be empty!
	if(boost::shared_ptr<ComputeEvent> compute_event =
	   boost::dynamic_pointer_cast<ComputeEvent> (event)) {
		list<boost::shared_ptr<Robot> >::iterator cur_robot =
		    computing_robots_.begin();
		while(cur_robot != computing_robots_.end()) {
			if(distribution_generator_->get_value_bernoulli()) {
				// Add the robot to the event.
				compute_event->add_to_robot_subset(*cur_robot);

				// Should add the robot to handling_robots_
				// and remove it from computing_robots_.
				handling_robots_.push_back(*cur_robot);
				cur_robot = computing_robots_.erase(cur_robot);
			} else {
				cur_robot++;
			}
		}

	} else if(boost::shared_ptr<LookEvent> look_event =
		      boost::dynamic_pointer_cast<LookEvent> (event)) {
		list<boost::shared_ptr<Robot> >::iterator cur_robot = looking_robots_.begin();
		while(cur_robot != looking_robots_.end()) {
			if(distribution_generator_->get_value_bernoulli()) {
				// add the robot to the event
				look_event->add_to_robot_subset(*cur_robot);

				// remove it from looking_robots_ and add it to computing_robots_
				computing_robots_.push_back(*cur_robot);
				cur_robot = looking_robots_.erase(cur_robot);
			} else {
				cur_robot++;
			}
		}

	} else if(boost::shared_ptr<HandleRequestsEvent> handle_requests_event =
		      boost::dynamic_pointer_cast<HandleRequestsEvent> (event)) {
		list<boost::shared_ptr<Robot> >::iterator cur_robot =
		    handling_robots_.begin();
		while(cur_robot != handling_robots_.end()) {
			if(distribution_generator_->get_value_bernoulli()) {
				// get all requests for this robot and add them to the event
				list<boost::shared_ptr<const Request> >::iterator cur_request =
				    unhandled_request_set_.begin();
				while(cur_request != unhandled_request_set_.end()) {
					// if the robot issued this request: add it to the event and delete it.
					if(&((*cur_request)->robot().id()) == &(*cur_robot)->id()) {
						handle_requests_event->add_to_requests(*cur_request);
						cur_request = unhandled_request_set_.erase(cur_request);
					} else {
						cur_request++;
					}
				}

				// remove it from handling_robots_ and add it to looking_robots_
				looking_robots_.push_back(*cur_robot);
				cur_robot = handling_robots_.erase(cur_robot);
			}
		}
	}

	// 3. Choose the time of the next event
	time_of_next_event_ = time_of_next_event_ +
	                      1 +
	                      distribution_generator_->get_value_geometric();
	return event;
}

void AsynchronousASG::update(TimePoint& time_point,
                            boost::shared_ptr<Event> last_event) {
	// Check what kind of event we got.
	// Only care for compute events since they contain unhandled requests.
	if(boost::shared_ptr<ComputeEvent> compute_event =
	   boost::dynamic_pointer_cast<ComputeEvent> (last_event)) {
		BOOST_FOREACH(boost::shared_ptr<const Request> cur_request,
				      compute_event->requests()) {
			unhandled_request_set_.push_back(cur_request);
		}
	}
}

boost::shared_ptr<Event> AsynchronousASG::choose_event_type() {
	if(looking_robots_.empty() && computing_robots_.empty()) {
		boost::shared_ptr<HandleRequestsEvent> result(
		    new HandleRequestsEvent(time_of_next_event_));
		return result;
	} else if(looking_robots_.empty() && handling_robots_.empty()) {
		boost::shared_ptr<ComputeEvent> result(new ComputeEvent(time_of_next_event_));
		return result;
	} else if(computing_robots_.empty() && handling_robots_.empty()) {
		boost::shared_ptr<LookEvent> result(new LookEvent(time_of_next_event_));
		return result;
	} else if(looking_robots_.empty()) {
		// choose between compute event and handle request event
		if(distribution_generator_->get_value_uniform() % 2 == 0) {
			boost::shared_ptr<ComputeEvent> result(
			    new ComputeEvent(time_of_next_event_));
			return result;
		} else {
			boost::shared_ptr<HandleRequestsEvent> result(
			    new HandleRequestsEvent(time_of_next_event_));
			return result;
		}
	} else if(computing_robots_.empty()) {
		// choose between look event and handle requests event
		if(distribution_generator_->get_value_uniform() % 2 == 0) {
			boost::shared_ptr<LookEvent> result(new LookEvent(time_of_next_event_));
			return result;
		} else {
			boost::shared_ptr<HandleRequestsEvent> result(
			    new HandleRequestsEvent(time_of_next_event_));
			return result;
		}
	} else if(handling_robots_.empty()) {
		// choose between look event and compute event
		if(distribution_generator_->get_value_uniform() % 2 == 0) {
			boost::shared_ptr<LookEvent> result(
			    new LookEvent(time_of_next_event_));
			return result;
		} else {
			boost::shared_ptr<ComputeEvent> result(
			    new ComputeEvent(time_of_next_event_));
			return result;
		}
	} else {
		// choose between all three kinds of events
		int random = distribution_generator_->get_value_uniform() % 3;
		if(random == 0) {
			boost::shared_ptr<LookEvent> result(
			    new LookEvent(time_of_next_event_));
			return result;
		} else if(random == 1) {
			boost::shared_ptr<ComputeEvent> result(
			    new ComputeEvent(time_of_next_event_));
			return result;
		} else if(random == 2) {
			boost::shared_ptr<HandleRequestsEvent> result(
			    new HandleRequestsEvent(time_of_next_event_));
			return result;
		} else {
			throw std::invalid_argument("random number was greater than 2");
		}
	}
}

#endif /* ASYNCHRONOUS_ASG_CC_ */

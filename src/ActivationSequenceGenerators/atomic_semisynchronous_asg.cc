/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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

#include "atomic_semisynchronous_asg.h"

#include "../Events/event.h"
#include "../Events/look_event.h"
#include "../Events/compute_event.h"
#include "../Events/handle_requests_event.h"
#include "../SimulationControl/history.h"
#include "../Model/robot_data.h"
#include "../Model/robot.h"

using namespace std;


void AtomicSemisynchronousASG::initialize(
	    const History& history,
	    const std::vector<boost::shared_ptr<Robot> >& robots,
	    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers) {
	// extract robots from robot data
	BOOST_FOREACH(boost::shared_ptr<Robot> robot, robots) {
		robots_.push_back(robot);
	}

	// Initialize the distribution generator
	// (needs to be done here since the size of robots is known here first).
	distribution_generator_->init_uniform(0, robots_.size() - 1);
}

boost::shared_ptr<Event> AtomicSemisynchronousASG::get_next_event() {
	boost::shared_ptr<Event> event;
	if(current_state_ == look) {
		LookEvent * look_event = new LookEvent(time_of_next_event_);
		event.reset(look_event);

		// add one random robot.
		current_robot_ = robots_.at(distribution_generator_->get_value_uniform());
		look_event->add_to_robot_subset(current_robot_);

		current_state_ = compute;

	} else if(current_state_ == compute) {
		ComputeEvent * compute_event = new ComputeEvent(time_of_next_event_);
		event.reset(compute_event);

		// add the current robot
		compute_event->add_to_robot_subset(current_robot_);

		current_state_ = move;

	} else if(current_state_ == move) {
		HandleRequestsEvent * handle_requests_event =
		    new HandleRequestsEvent(time_of_next_event_);
		event.reset(handle_requests_event);

		// Copy over all requests (there might be more than one if the
		// current robot issued more than one).
		BOOST_FOREACH(boost::shared_ptr<const Request> cur_request,
				      unhandled_request_set_) {
			handle_requests_event->add_to_requests(cur_request);
		}
		unhandled_request_set_.clear();

		// advance the time, state
		current_state_ = look;
		time_of_next_event_++;
	}

	return event;
}

void AtomicSemisynchronousASG::update(TimePoint& time_point,
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

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

/*
 * asynchronous_asg.h
 *
 *  Created on: Jan 24, 2009
 *      Author: craupach
 */

#ifndef ASYNCHRONOUS_ASG_H_
#define ASYNCHRONOUS_ASG_H_

#include <vector>
#include <list>

#include <boost/smart_ptr.hpp>
#include <boost/circular_buffer.hpp>

#include "activation_sequence_generator.h"
#include "../Model/world_information.h"
#include "../Events/look_event.h"
#include "../Requests/request.h"
#include "../Utilities/distribution_generator.h"


// forward declarations
class History;

/**
 * \class AsynchronousASG
 * \brief The asynchronous ASG tries to produce a sequence of events challenging to algorithms developed
 * for the asynchronous time model. Nevertheless it is of course not equivalent to the asynchronous time
 * model.
 *
 * The sequence produced by the asynchronous ASG satisfies the following invariants:
 * 1. There are never two events for the same point in time.
 * 2. The order of events for a fixed robot will always be: Look-Compute-HandleRequests
 * 3. In a infinite sequence each robot looks, computes and moves an infinite number of times.
 */
class AsynchronousASG : public ActivationSequenceGenerator {
public:
	/**
	 * declare a friend class for doing unit tests with this class.
	 */
	friend class AsynchronousASGTestAccessor;

	AsynchronousASG(unsigned int seed,
	                double participation_probability,
	                double p);

	/**
	 * initializes the asynchronous ASG from the given intial world_state. Needs to be called before the
	 * ASG is used
	 * \param The intial world state
	 */
	void initialize(
		    const History& history,
		    const std::vector<boost::shared_ptr<Robot> >& robots,
		    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers = *new std::vector<boost::shared_ptr<WorldModifier> >()); //TODO: Change test cases to specify world modifiers


	/**
	 * Returns the next event.
	 * \return The next event in the sequence.
	 */
	boost::shared_ptr<Event> get_next_event();

	/**
	 * Returns the time of the next event. This is computed according to...
	 * \return The time of the next event.
	 */
	int get_time_of_next_event() { return time_of_next_event_; }

	/**
	 * Updates the sequence of events. Ensures that the events for each fixed robot are in the right
	 * order
	 * \param A constant refrence to the newest world information
	 * \param The last handled event
	 */
	void update(TimePoint& time_point,
			    boost::shared_ptr<Event> event);

private:
	/**
	 * chooses, which kind of event will happen next.
	 */
	boost::shared_ptr<Event> choose_event_type();

	/**
	 * The time the next event will happen.
	 */
	int time_of_next_event_;

	/**
	 * A set of unhandled requests from some compute events in the past.
	 */
	std::list<boost::shared_ptr<const Request> > unhandled_request_set_;

	/**
	 * The set of all robots which will have a look event as their next event.
	 */
	std::list<boost::shared_ptr<Robot> > looking_robots_;

	/**
	 * The set of all robots which will have a compute event as their next event.
	 */
	std::list<boost::shared_ptr<Robot> > computing_robots_;

	/**
	 * The set of all robots which will have handle requests event as their next event.
	 */
	std::list<boost::shared_ptr<Robot> > handling_robots_;

	/**
	 * a source of randomness
	 */
	boost::shared_ptr<DistributionGenerator> distribution_generator_;
};


#endif /* ASYNCHRONOUS_ASG_H_ */

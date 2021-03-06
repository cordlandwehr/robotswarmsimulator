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
 * atomic_semisynchronous_asg.h
 *
 *  Created on: Apr 8, 2009
 *      Author: craupach
 */

#ifndef ATOMIC_SEMISYNCHRONOUS_ASG_H_
#define ATOMIC_SEMISYNCHRONOUS_ASG_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include "../Utilities/distribution_generator.h"

#include "activation_sequence_generator.h"

class Robot;
class Event;
class WorldInformation;

/**
 * The atomic semisynchronous ASG generates a complete LCM cycle for a single random robot each timestep.
 * At each timestep a robot is choosen uniform at random from all robots.
 * That robots gets a look event, then a compute event, then a move event all in the same time step
 * After the move event the time is advanced a single step
 */
class AtomicSemisynchronousASG : public ActivationSequenceGenerator {
friend class atomic_semisynchronous_asg_smoke_test;

public:
	explicit AtomicSemisynchronousASG(unsigned int seed) :
	    time_of_next_event_(0),
	    current_state_(AtomicSemisynchronousASG::look),
	    distribution_generator_(new DistributionGenerator(seed)) {}
	/**
	 * Initializes the ASG.
	 */
	void initialize(
		    const History& history,
		    const std::vector<boost::shared_ptr<Robot> >& robots,
		    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers = *new std::vector<boost::shared_ptr<WorldModifier> >());
	
	/**
	 * Returns the next event.
	 * \return The next event produced by the ASG
	 */
	boost::shared_ptr<Event> get_next_event();

	/**
	 * Returns the time the next event happens
	 * \return Integer representing the next time an event will happen
	 */
	int get_time_of_next_event() { return time_of_next_event_; }

	/**
	 * Updates the sequence of events. For the atomic semisynchronous ASG this only stores the requests of robots
	 * stored in compute events. The requests are added to the next handle_requests event.
	 * \param A constant refrence to the newest world information
	 * \param The last handled event
	 */
	void update(TimePoint& time_point,
			    boost::shared_ptr<Event> event);

private:
	/**
	 * enum for the state the asg is currently in (determines kind of next event)
	 */
	enum State {
		look,
		compute,
		move
	};

	int time_of_next_event_;
	State current_state_;
	boost::shared_ptr<Robot> current_robot_;

	/**
	 * The set of all robots
	 */
	std::vector<boost::shared_ptr<Robot> > robots_;

	/**
	 * A set of unhandled requests from the last compute event.
	 */
	std::vector<boost::shared_ptr<const Request> > unhandled_request_set_;

	/**
	 * a source of randomness
	 */
	boost::shared_ptr<DistributionGenerator> distribution_generator_;
};


#endif /* ATOMIC_SEMISYNCHRONOUS_ASG_H_ */

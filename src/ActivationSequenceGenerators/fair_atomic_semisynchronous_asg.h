/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

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
 * fair_atomic_semisynchronous_asg.h
 *
 *  Created on: Aug 2, 2009
 *      Author: dwonisch
 */

#ifndef FAIR_ATOMIC_SEMISYNCHRONOUS_ASG_H_
#define FAIR_ATOMIC_SEMISYNCHRONOUS_ASG_H_

#include <vector>
#include <list>
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include "../Utilities/distribution_generator.h"

#include "activation_sequence_generator.h"

class Robot;
class Event;
class WorldInformation;

/**
 * The fair atomic semisynchronous ASG generates a complete LCM cycle for a single random robot each timestep.
 * In constrast to unfair atomic semisynchronous ASG it also ensures that every robot is activated exaclty once,
 * every n timesteps (where n is the number of robots).
 */
class FairAtomicSemisynchronousASG : public ActivationSequenceGenerator {
public:
	explicit FairAtomicSemisynchronousASG(unsigned int seed) :
	    time_of_next_event_(0),
	    current_state_(FairAtomicSemisynchronousASG::look),
	    distribution_generator_(new DistributionGenerator(seed)) {}

	/**
	 * Initializes the ASG.
	 */
	void initialize(
		    const History& history,
		    const std::vector<boost::shared_ptr<Robot> >& robots,
		    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers = *new std::vector<boost::shared_ptr<WorldModifier> >());

	void removeRobot(const RobotIdentifier& robot_identifier) {}; //TODO has to be implemented
	
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
	 * The set of robots not activated since the last activation round.
	 */
	std::list<boost::shared_ptr<Robot> > unactivated_robots_;

	/**
	 * A set of unhandled requests from the last compute event.
	 */
	std::vector<boost::shared_ptr<const Request> > unhandled_request_set_;

	/**
	 * a source of randomness
	 */
	boost::shared_ptr<DistributionGenerator> distribution_generator_;
};


#endif /* FAIR_ATOMIC_SEMISYNCHRONOUS_ASG_H_ */

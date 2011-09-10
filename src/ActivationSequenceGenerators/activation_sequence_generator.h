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
 * activation_sequence_generator.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef ACTIVATION_SEQUENCE_GENERATOR_H_
#define ACTIVATION_SEQUENCE_GENERATOR_H_

#include <boost/smart_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include <vector>

#include "../Model/world_information.h"
#include "../Events/look_event.h"
#include "../SimulationKernel/simulation_listener.h"
#include "../Model/world_modifier.h"


// forward declarations
class History;

/**
 * \class ActivationSequenceGenerator
 * \brief Interface for activation sequence generators
 *
 * The activation sequence generator (AGS) decides, according to some user--specified rules,
 * how to time the different events. In other words, it manages the timing of the execution
 * of the robot algorithms.
 *
 * The ActivationSequenceGenerator class inherits from the SimulationListener interface.
 *
 */
class ActivationSequenceGenerator : public SimulationListener {
public:

	virtual ~ActivationSequenceGenerator() {}

	/**
	 * Initializes the ASG.
	 * \param The history
	 */
	virtual void initialize(
	    const History& history,
	    const std::vector<boost::shared_ptr<Robot> >& robots,
	    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers) = 0;

	/**
	 * Returns the next event.
	 * \return The next event produced by the ASG
	 */
	virtual boost::shared_ptr<Event> get_next_event() = 0;

	/**
	 * Returns the time the next event happens
	 * \return Integer representing the next time an event will happen
	 */
	virtual int get_time_of_next_event() = 0;
	
		/**
	 * removes a robot from the list of set of robots stored in this ASG
	 * \param robot_identifier ID of the robot to be removed
	*/
	virtual void removeRobot(const RobotIdentifier& robot_identifier) = 0;
};

#endif /* ACTIVATION_SEQUENCE_GENERATOR_H_ */

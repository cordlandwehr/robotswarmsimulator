/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 * robot_control.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef ROBOT_CONTROL_H_
#define ROBOT_CONTROL_H_

#include <set>
#include <boost/shared_ptr.hpp>
#include "simulation_listener.h"

class Request;
class Robot;

/**
 * \class RobotControl
 *
 * Controls the Robots. Responsible for assigning the View to each Robot and
 * calling the compute method when requested.
 *
 * RobotControl most likely holds a buffer of Views of length history_length. To do this, the
 * RobotControl provides an update method.
 *
 */
class RobotControl {
public:
	/**
	 * Constructs a new RobotControl.
	 * @param The view_factory given determines which
	 * view model should be used for the robots
	 * @param The length of the history
	 * @param intial WorldInformation
	 * @see ModelParameters::HISTORY_LENGTH
	 */
	RobotControl();
	virtual ~RobotControl();

	virtual void update(const boost::shared_ptr<WorldInformation>& world_information) = 0;

	/**
	 * Equivalent to robot.compute().
	 * @param robot
	 * @param Set of Requests
	 * @see Robot::compute()
	 */
	std::set<boost::shared_ptr<Request> > compute_new_request(Robot& robot);
	/**
	 * Computes and assigns the newest View to the given robot.
	 * @param robot
	 */
	virtual void compute_view(Robot& robot) = 0;
};

#endif /* ROBOT_CONTROL_H_ */

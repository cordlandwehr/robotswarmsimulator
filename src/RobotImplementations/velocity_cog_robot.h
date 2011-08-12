/*
	This file is part of RobotSwarmSimulator.

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
 * velocity_cog_robot.h
 *
 *  Created on: Feb 28, 2009
 *      Author: craupach
 */

#ifndef VELOCITY_COG_ROBOT_H_
#define VELOCITY_COG_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

#include <iostream>

class VelocityCOGRobot : public Robot {
public:
	VelocityCOGRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "VelocityCOGRobot";
	}
};


#endif /* VELOCITY_COG_ROBOT_H_ */

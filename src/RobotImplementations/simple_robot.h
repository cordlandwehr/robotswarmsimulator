/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
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
 * simple_robot.h
 *
 *  Created on: Feb 10, 2009
 *      Author: craupach
 */

#ifndef SIMPLE_ROBOT_H_
#define SIMPLE_ROBOT_H_

#include "../Model/robot.h"

class SimpleRobot : public Robot {
public:
	SimpleRobot(boost::shared_ptr<RobotIdentifier> id)
			: Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute() {

		// don't do stuff
		std::set<boost::shared_ptr<Request> > result_set;

		return result_set;
	}

	virtual std::string get_algorithm_id () const {
		return "SimpleRobot";
	}
};


#endif /* SIMPLE_ROBOT_H_ */

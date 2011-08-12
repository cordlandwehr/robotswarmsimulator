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
 * request.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "../Model/robot.h"

/**
 * \class Request
 * \brief Class which represents a request of a robot.
 *
 */
class Request {
public:
	explicit Request(Robot& robot) : robot_(robot) {}
	virtual ~Request() = 0;

	/**
	 * returns a constant reference to the robot which issued the request
	 * \Return a constant reference to the robot which issued the request
	 */
	const Robot& robot() const { return robot_; }

private:
	/**
	 * The robot which issued the request.
	 */
	Robot& robot_;
};

#endif /* REQUEST_H_ */

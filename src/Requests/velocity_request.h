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
 * velocity_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef VELOCITY_REQUEST_H_
#define VELOCITY_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "vector_request.h"

/**
 * \brief A velocity request is issued by a robot which wants to change its velocity to a new value
 *
 * Notes:
 * The new velocity is expressed in terms of the local coordinate system of the robot. This means it
 * has to be transformed before using.
 *
 * The request cannot be changed after construction.
 */
class VelocityRequest : public VectorRequest {
public:
	VelocityRequest(Robot& robot, boost::shared_ptr<Vector3d> requested_vector) :
		    VectorRequest(robot, requested_vector) {}
};

#endif /* VELOCITY_REQUEST_H_ */

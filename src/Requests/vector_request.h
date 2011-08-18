/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
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
 * vector_request.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef VECTOR_REQUEST_H_
#define VECTOR_REQUEST_H_

#include "../Utilities/vector_arithmetics.h"
#include "request.h"

class VectorRequest : public Request {
public:
	/**
	 * constructs a new VectorRequest.
	 * The request cannot be changed after construction.
	 */
	VectorRequest(Robot& robot, boost::shared_ptr<Vector3d> requested_vector) :
	    Request(robot), requested_vector_(requested_vector) {}

	/**
	 * Returns a constant reference to the requested vector
	 * \Return A constant reference to the requested vector
	 */
	const Vector3d& requested_vector() const {return *requested_vector_;}

private:
	/**
	 * the requested vector expressed in terms of the local coordinate system of the robot
	 */
	boost::shared_ptr<Vector3d> requested_vector_;
};


#endif /* VECTOR_REQUEST_H_ */

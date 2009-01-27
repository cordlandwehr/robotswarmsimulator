/*
 * position_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef POSITION_REQUEST_H_
#define POSITION_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "request.h"
#include "../Utilities/vector3d.h"

/**
 * \brief A position request is issued by a robot which wants to change its position to a new value
 *
 * Notes:
 * The new position is expressed in terms of the local coordinate system of the robot. This means it
 * has to be transformed before using.
 *
 * The request cannot be changed after construction.
 */
class PositionRequest : public Request{
public:
	/**
	 * constructs a new Position Request.
	 * The request cannot be changed after construction.
	 */
	PositionRequest(Robot& robot, boost::shared_ptr<Vector3d> requested_position) :
	               Request(robot), requested_position_(requested_position) {}

	/**
	 * Returns a constant reference to the requested position
	 * \Return A constant reference to the requested position
	 */
	const Vector3d& requested_position() const {return *requested_position_;}

private:
	/**
	 * the requested position expressed in terms of the local coordinate system of the robot
	 */
	boost::shared_ptr<Vector3d> requested_position_;
};


#endif /* POSITION_REQUEST_H_ */

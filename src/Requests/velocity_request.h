/*
 * velocity_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef VELOCITY_REQUEST_H_
#define VELOCITY_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "request.h"
#include "../Utilities/vector3d.h"

/**
 * \brief A velocity request is issued by a robot which wants to change its velocity to a new value
 *
 * Notes:
 * The new velocity is expressed in terms of the local coordinate system of the robot. This means it
 * has to be transformed before using.
 *
 * The request cannot be changed after construction.
 */
class VelocityRequest : public Request {
public:
	/**
	 * constructs a new Velocity Request.
	 * The request cannot be changed after construction.
	 */
	VelocityRequest(boost::shared_ptr<Robot> robot, boost::shared_ptr<Vector3d> requested_velocity) :
	               Request(robot), requested_velocity_(requested_velocity) {}

	/**
	 * Returns a constant reference to the requested velocity
	 * \Return A constant reference to the requested velocity
	 */
	const Vector3d& requested_velocity() const {return *requested_velocity_;}

private:
	/**
	 * requested velocity expressed in terms of the local coordinate system of the robot.
	 */
	boost::shared_ptr<Vector3d> requested_velocity_;
};

#endif /* VELOCITY_REQUEST_H_ */

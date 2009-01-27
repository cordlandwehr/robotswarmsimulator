/*
 * acceleration_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef ACCELERATION_REQUEST_H_
#define ACCELERATION_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "request.h"
#include "../Utilities/vector3d.h"

/**
 * \brief An Acceleration Request is issued by a robot which wants to change its acceleration to a new value
 *
 * Notes:
 * The new acceleration is expressed in terms of the local coordinate system of the robot. This means it
 * has to be transformed before using.
 *
 * The request cannot be changed after construction.
 */
class AccelerationRequest : public Request {
public:
	/**
	 * constructs a new AccelerationRequest.
	 * The request cannot be changed after construction.
	 */
	AccelerationRequest(Robot& robot,
			            boost::shared_ptr<Vector3d> new_acceleration) : Request(robot), requested_acceleration_(new_acceleration) {}
	/**
	 * Returns a constant reference to the requested acceleration vector
	 * \Return A Constant reference to the requested acceleration vector
	 */
	const Vector3d& requested_acceleration() const { return *requested_acceleration_;}

private:
	/**
	 * the new acceleration expressed in terms of the local coordinate system of the robot
	 */
	boost::shared_ptr<Vector3d> requested_acceleration_;

};

#endif /* ACCELERATION_REQUEST_H_ */

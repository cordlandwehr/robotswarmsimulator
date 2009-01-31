/*
 * acceleration_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef ACCELERATION_REQUEST_H_
#define ACCELERATION_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "vector_request.h"

/**
 * \brief An Acceleration Request is issued by a robot which wants to change its acceleration to a new value
 *
 * Notes:
 * The new acceleration is expressed in terms of the local coordinate system of the robot. This means it
 * has to be transformed before using.
 *
 * The request cannot be changed after construction.
 */
class AccelerationRequest : public VectorRequest {
public:
	AccelerationRequest(Robot& robot, boost::shared_ptr<Vector3d> requested_vector) :
		    VectorRequest(robot, requested_vector) {}
};

#endif /* ACCELERATION_REQUEST_H_ */

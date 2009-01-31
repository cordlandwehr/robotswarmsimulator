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

/*
 * position_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef POSITION_REQUEST_H_
#define POSITION_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "vector_request.h"

/**
 * \brief A position request is issued by a robot which wants to change its position to a new value
 *
 * Notes:
 * The new position is expressed in terms of the local coordinate system of the robot. This means it
 * has to be transformed before using.
 *
 * The request cannot be changed after construction.
 */
class PositionRequest : public VectorRequest {
public:
	PositionRequest(Robot& robot, boost::shared_ptr<Vector3d> requested_vector) :
	    VectorRequest(robot, requested_vector) {}

};


#endif /* POSITION_REQUEST_H_ */

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

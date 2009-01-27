/*
 * TypeChangeRequest.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef TYPECHANGEREQUEST_H_
#define TYPECHANGEREQUEST_H_

#include <boost/smart_ptr.hpp>
#include "request.h"
#include "../Model/robot_data.h"

/**
 * \brief A Type Change Request is issued by a robot which wants to change its type (e.g. become leader)
 *
 * Notes:
 * This request differs from the other request classes since it doesn't take a pointer to the requested
 * type but rather the type directly. Since Type is only a small enum we don't need the overhead of the
 * shared pointer.
 *
 * The request cannot be changed after creating.
 */
class TypeChangeRequest : public Request {
public:
	/**
	 * Constructs a new type change request.
	 * The request cannot be changed after construction.
	 */
	TypeChangeRequest(Robot& robot, RobotType requested_type) : Request(robot),
	                                                                         requested_type_(requested_type) {}

	/**
	 * Returns a constant reference to the requested type
	 * \return A constant reference to the requested type
	 */
	const RobotType& requested_type() const {return requested_type_;}

private:
	RobotType requested_type_;
};

#endif /* TYPECHANGEREQUEST_H_ */

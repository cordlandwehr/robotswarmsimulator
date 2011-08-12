/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 * marker_change_request.h
 *
 *  Created on: Feb 25, 2009
 *      Author: peter
 */

#ifndef MARKER_CHANGE_REQUEST_H_
#define MARKER_CHANGE_REQUEST_H_

#include <string>

#include <boost/any.hpp>

#include "request.h"
#include "../Model/marker_identifier.h"


/**
 * \brief May be issued by a robot to change a specified marker object.
 *
 * Using this request, robots may request to add or remove a specific data item from a specific marker object.
 */
class MarkerChangeRequest : public Request {
public:
	/**
	 * \brief Constructs a new MarkerChangeRequest.
	 *
	 * The request cannot be changed after constructing.
	 *
	 * \param marker_to_change MarkerIdentifier of marker for that a change is requested.
	 * \param var_name Variable name of the data-item to be added/removed.
	 * \param var_value Pointer to the value. If this pointer is valid, the request will set var_name of
	 *                  marker_to_change to the data object pointed to by var_value. If the var_value pointer is not
	 *                  valid, the removal of data item var_name will be requested.
	 */
	MarkerChangeRequest(Robot& robot, MarkerIdentifier& marker_to_change,
	                    const std::string& var_name,
	                    boost::shared_ptr<const boost::any> var_value)
	: Request(robot),
	  marker_to_change_(marker_to_change),
	  var_name_(var_name),
	  var_value_(var_value) {}

	/**
	 * \brief Returns a constant reference to the identifier of the marker that was requsted to be changed.
	 * \return A constant reference to the identifier of the marker that was requsted to be changed.
	 */
	const MarkerIdentifier& marker_to_change() const { return marker_to_change_; }

	/**
	 * \brief Returns the variable name to be changed.
	 */
	const std::string& var_name() const { return var_name_; }

	/**
	 * \brief Returns a pointer to the new data requested for var_name_.
	 *
	 * Note that this pointer may be invalid, indicating that the request creator requested the deletion of the given
	 * data item var_name_.
	 */
	boost::shared_ptr<const boost::any> var_value() const { return var_value_; }

private:
	const MarkerIdentifier& marker_to_change_;
	const std::string& var_name_;
	boost::shared_ptr<const boost::any> var_value_;
};

#endif /* MARKER_CHANGE_REQUEST_H_ */

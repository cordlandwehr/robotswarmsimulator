/*
	This file is part of RobotSwarmSimulator.

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
 * marker_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef MARKER_REQUEST_H_
#define MARKER_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "request.h"
#include "../Model/marker_information.h"

/**
 * \brief A Marker Request is issued by a robot which wants to change its marker information.
 *
 * Notes:
 * The request cannot be changed after creating.
 */
class MarkerRequest : public Request {
public:
	/**
	 * constructs a new MarkerRequest.
	 * The request cannot be changed after constructing.
	 */
	MarkerRequest(
	    Robot& robot,
	    boost::shared_ptr<MarkerInformation> requested_marker_information) :
	        Request(robot),
	        requested_marker_information_(requested_marker_information) {}

	/**
	 * Returns a constant reference to the requested marker information
	 * \Return A constant reference to the requested marker information
	 */
	const MarkerInformation& requested_marker_information() const {
		return *requested_marker_information_;
	}

private:
	boost::shared_ptr<MarkerInformation> requested_marker_information_;
};

#endif /* MARKER_REQUEST_H_ */

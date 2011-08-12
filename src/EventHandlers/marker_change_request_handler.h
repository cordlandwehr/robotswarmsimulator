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
 * marker_change_request_handler.h
 *
 *  Created on: Feb 25, 2009
 *      Author: peter
 */

#ifndef MARKER_DELETE_REQUEST_HANDLER
#define MARKER_DELETE_REQUEST_HANDLER

#include "../EventHandlers/request_handler.h"


/**
 * \brief Handles MarkerChangeRequests by changing the specified marker as requested.
 */
class MarkerChangeRequestHandler : public RequestHandler {
public:
	MarkerChangeRequestHandler(unsigned int seed,
	                           double discard_probability,
	                           const History& history)
	: RequestHandler(seed, discard_probability, history) { }

	virtual ~MarkerChangeRequestHandler() { }

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* MARKER_CHANGE_REQUEST_HANDLER */

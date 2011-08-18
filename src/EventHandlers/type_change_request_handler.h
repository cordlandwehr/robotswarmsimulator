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
 * type_change_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef TYPE_CHANGE_REQUEST_HANDLER_H_
#define TYPE_CHANGE_REQUEST_HANDLER_H_

#include "../EventHandlers/request_handler.h"

/**
 * Standard type change request handler. Fullfills every type change request.
 */
class TypeChangeRequestHandler : public RequestHandler {
	friend class LoadMainProjectFileTest;
public:
	TypeChangeRequestHandler(
	    unsigned int seed,
	    double discard_probability,
	    const History& history) : RequestHandler(seed,
	                                             discard_probability,
	                                             history) {}
    virtual ~TypeChangeRequestHandler() {}

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* TYPE_CHANGE_REQUEST_HANDLER_H_ */

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
 * type_change_request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#include "../EventHandlers/type_change_request_handler.h"

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/type_change_request.h"

bool TypeChangeRequestHandler:: handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {
	boost::shared_ptr<const TypeChangeRequest> type_change_request =
	     boost::dynamic_pointer_cast<const TypeChangeRequest> (request);
	if(!type_change_request) {
		throw std::invalid_argument("Not a type change request.");
	}

	const boost::shared_ptr<RobotIdentifier>& robot_id =
	    type_change_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	robot_data.set_type(type_change_request->requested_type());
	return true;
}


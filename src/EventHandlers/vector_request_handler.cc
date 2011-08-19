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
 * vector_request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#include "../EventHandlers/vector_request_handler.h"

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/request.h"
#include "../Requests/vector_request.h"
#include "../Requests/position_request.h"


#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/VectorModifiers/vector_modifier.h"

using boost::shared_ptr;

bool VectorRequestHandler::handle_request_reliable(
    shared_ptr<WorldInformation> world_information,
    shared_ptr<const Request> request) {
	shared_ptr<const VectorRequest> vector_request =
	    boost::dynamic_pointer_cast<const VectorRequest> (request);
	if(!vector_request) {
		throw std::invalid_argument("Not a vector request.");
	}
    const shared_ptr<RobotIdentifier>& robot_id = vector_request->robot().id();
    RobotData& robot_data =
        world_information->get_according_robot_data(robot_id);
	Vector3d global_vector = vector_request->requested_vector();
	//extract_global_vector(*vector_request, robot_data); //TODO asetzer: if anything crashes, this change was probably wrong
	const Vector3d& reference_vector = extract_ref_vector(*vector_request,
	                                                      robot_data);

    // apply vector modifiers from pipeline
	bool vector_changed = false;
	BOOST_FOREACH(shared_ptr<VectorModifier>& vector_modifier, vector_modifiers_) {
		vector_changed = vector_modifier->modify_vector(global_vector,
		                                                reference_vector);
	}

	// update corresponding robot property
    update_vector(*vector_request, robot_data, global_vector);
    // return false if the request was not performed exactly as requested
	return !vector_changed;
}

void VectorRequestHandler::update_vector(const VectorRequest& request,
                                         RobotData& robot_data,
                                         const Vector3d& vector) {
	shared_ptr<Vector3d> vector_ptr(new Vector3d(vector));
	if (typeid(request) == typeid(PositionRequest))
		robot_data.set_position(vector_ptr);
	else
		throw std::invalid_argument("VectorRequestHandler: unknown vector request; can not update robot's vector");
}


const Vector3d& VectorRequestHandler::extract_ref_vector(
    const VectorRequest& request,
    const RobotData& robot_data) {
	if (typeid(request) == typeid(PositionRequest))
		return robot_data.position();
	else
		throw std::invalid_argument("VectorRequestHandler: unknown vector request, can not extract reference vector");
}

void VectorRequestHandler::add_vector_modifier(
    shared_ptr<VectorModifier> vector_modifier) {
	vector_modifiers_.push_back(vector_modifier);
}

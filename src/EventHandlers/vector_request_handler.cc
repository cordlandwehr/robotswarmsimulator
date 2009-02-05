/*
 * vector_request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include "vector_modifier.h"

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/request.h"
#include "../Requests/vector_request.h"
#include "../Requests/position_request.h"
#include "../Requests/velocity_request.h"
#include "../Requests/acceleration_request.h"

#include "../Utilities/coord_converter.h"
#include "../Utilities/vector3d.h"

#include "vector_request_handler.h"

void VectorRequestHandler::handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                                    boost::shared_ptr<const Request> request) {
	 boost::shared_ptr<const VectorRequest> vector_request =
	 	     boost::dynamic_pointer_cast<const VectorRequest> (request);
	 if(!vector_request) {
		 throw std::invalid_argument("Not a vector request.");
	 }
    const boost::shared_ptr<RobotIdentifier>& robot_id = vector_request->robot().id();
    RobotData& robot_data = world_information->get_according_robot_data(robot_id);
    const Vector3d& local_vector(vector_request->requested_vector());
    // TODO(craupach) this is a problem: For velocity and acceleration we should set the origin to (0,0,0)
	boost::shared_ptr<Vector3d> global_vector = CoordConverter::local_to_global(local_vector, robot_data.position(),
																				robot_data.coordinate_system_axis());
	const Vector3d global_vector_cpy(*global_vector);

    // apply vector modifiers from pipeline
	BOOST_FOREACH(boost::shared_ptr<VectorModifier>& vector_modifier, vector_modifiers_) {
		vector_modifier->modify_vector(*global_vector, global_vector_cpy);
	}

	// apply request
    apply_request(vector_request, robot_data, global_vector);
}

void VectorRequestHandler::apply_request(boost::shared_ptr<const VectorRequest> vector_request, RobotData & robot_data, boost::shared_ptr<Vector3d> processed_global_vector)
{
    if(boost::shared_ptr<const PositionRequest> position_request = boost::dynamic_pointer_cast<const PositionRequest>(vector_request)){

        robot_data.set_position(processed_global_vector);
    }else if(boost::shared_ptr<const VelocityRequest> velocity_request = boost::dynamic_pointer_cast<const VelocityRequest>(vector_request)){

        robot_data.set_velocity(processed_global_vector);
    } else if(boost::shared_ptr<const AccelerationRequest> acceleration_request = boost::dynamic_pointer_cast<const AccelerationRequest>(vector_request)){

        robot_data.set_acceleration(processed_global_vector);
    }
}

void VectorRequestHandler::add_vector_modifier(boost::shared_ptr<VectorModifier> vector_modifier) {
	vector_modifiers_.push_back(vector_modifier);
}

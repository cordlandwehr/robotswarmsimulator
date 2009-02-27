/*
 * vector_request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

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
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/VectorModifiers/vector_modifier.h"

#include "vector_request_handler.h"


void VectorRequestHandler::handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                                    boost::shared_ptr<const Request> request) {
	 boost::shared_ptr<const VectorRequest> vector_request = boost::dynamic_pointer_cast<const VectorRequest> (request);
	 if(!vector_request) {
		 throw std::invalid_argument("Not a vector request.");
	 }
    const boost::shared_ptr<RobotIdentifier>& robot_id = vector_request->robot().id();
    RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	Vector3d global_vector = extract_global_vector(*vector_request, robot_data);
	// const Vector3d global_vector_cpy(global_vector); Wrong for vector difference trimmer!
	const Vector3d reference_vector = robot_data.position(); // TODO(craupach) should set correct reference vector (not always the position!)

    // apply vector modifiers from pipeline
	BOOST_FOREACH(boost::shared_ptr<VectorModifier>& vector_modifier, vector_modifiers_) {
		vector_modifier->modify_vector(global_vector,reference_vector);
	}

	// update corresponding robot property
    update_vector(*vector_request, robot_data, global_vector);
}

void VectorRequestHandler::update_vector(const VectorRequest& request, RobotData& robot_data, const Vector3d& vector) {
	boost::shared_ptr<Vector3d> vector_ptr(new Vector3d(vector));
	if (typeid(request) == typeid(PositionRequest))
		robot_data.set_position(vector_ptr);
	else if (typeid(request) == typeid(VelocityRequest))
		robot_data.set_velocity(vector_ptr);
	else if (typeid(request) == typeid(AccelerationRequest))
		robot_data.set_acceleration(vector_ptr);
}

Vector3d VectorRequestHandler::extract_global_vector(const VectorRequest& request, const RobotData& robot_data) {
	using CoordConverter::local_to_global;
	using boost::shared_ptr;

    const Vector3d& local_vector(request.requested_vector());
	Vector3d position(robot_data.position());
	if (typeid(request) == typeid(VelocityRequest) || typeid(request) == typeid(AccelerationRequest))
		position = boost::numeric::ublas::zero_vector<double>(3);
	shared_ptr<Vector3d> global_vector = local_to_global(local_vector, position, robot_data.coordinate_system_axis());
	return *global_vector;
}

void VectorRequestHandler::add_vector_modifier(boost::shared_ptr<VectorModifier> vector_modifier) {
	vector_modifiers_.push_back(vector_modifier);
}

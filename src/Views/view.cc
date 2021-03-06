/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

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

#include "view.h"


#include <boost/lambda/lambda.hpp>

#include "../Model/identifier.h"
#include "../Model/robot_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/marker_identifier.h"
#include "../Model/box_identifier.h"
#include "../Model/sphere_identifier.h"
#include "../Utilities/vector_arithmetics.h"
#include "../Model/marker_information.h"
#include "../Model/robot.h"
#include "../Model/obstacle.h"
#include "../Model/box.h"
#include "../Model/sphere.h"
#include "../Model/world_information.h"
#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/distribution_generator.h"


boost::shared_ptr<DistributionGenerator> View::generator_ = boost::shared_ptr<DistributionGenerator>();

void View::set_distribution_generator(boost::shared_ptr<DistributionGenerator> generator) {
	generator_ = generator;
	generator_->init_uniform(0, std::numeric_limits<int>::max());
}


View::View() {
}

View::~View() {	
}

namespace {
	void validate_identifier(const WorldObject& obj, boost::shared_ptr<Identifier> identifier) {
		if(obj.id() != identifier) {
			throw std::invalid_argument("Illegal identifier. Outdated? Cloned? Self-created?");
		}
	}

	std::string get_error_message(std::string method_name) {
		return method_name + " not implemented in this model.";
	}

}

template<typename T>
T View::delegate_function(boost::function<T (const View*, const RobotData&)> own_robot_fun,
		                  boost::function<T (const View*, const RobotData&)> other_robot_fun,
						  boost::function<T (const View*, const Obstacle&)> obstacle_fun,
						  boost::function<T (const View*, const WorldObject&)> marker_fun,
						  boost::function<T (const View*, const Edge&)> edge_fun,
						  boost::function<T (const View*, const Message&)> message_fun,
						  const Robot& caller, WorldObjectRef world_object) const {
	if(RobotRef ref = boost::dynamic_pointer_cast<RobotIdentifier>(world_object)) {
		if(is_own_identifier(caller, ref)) {
			return own_robot_fun(this, resolve_robot_ref(ref));
		} else {
			return other_robot_fun(this, resolve_robot_ref_safe(ref));
		}
	} else if(ObstacleRef ref = boost::dynamic_pointer_cast<ObstacleIdentifier>(world_object)) {
		return obstacle_fun(this, resolve_obstacle_ref_safe(ref));
	} else if(MarkerRef ref = boost::dynamic_pointer_cast<MarkerIdentifier>(world_object)) {
		return marker_fun(this, resolve_marker_ref_safe(ref));
	} else if(EdgeRef ref = boost::dynamic_pointer_cast<EdgeIdentifier>(world_object)) {
		return edge_fun(this, resolve_edge_ref_safe(ref));
	} else if(MessageRef ref = boost::dynamic_pointer_cast<MessageIdentifier>(world_object)) {
		return message_fun(this, resolve_message_ref_safe(ref));
	} else {
		throw std::invalid_argument("Illegal type of world_object.");
	}
}

template<typename T>
T View::delegate_function(boost::function<T (const View*, const RobotData&)> own_robot_fun,
		                  boost::function<T (const View*, const RobotData&)> other_robot_fun,
		                  const Robot& caller, RobotRef robot) const {
	if(is_own_identifier(caller, robot)) {
		return own_robot_fun(this, resolve_robot_ref(robot));
	} else {
		return other_robot_fun(this, resolve_robot_ref_safe(robot));
	}
}

bool View::is_own_identifier(const Robot& robot, boost::shared_ptr<RobotIdentifier> identifier) {
	return robot.id()->id() == identifier->id();
}

const Obstacle& View::resolve_obstacle_ref(ObstacleRef obstacle) const {
	return *(world_information().obstacles())[obstacle->id()];
}
const RobotData& View::resolve_robot_ref(RobotRef robot) const {
	return *(world_information().robot_data().at(robot->id()));
}
const WorldObject& View::resolve_marker_ref(MarkerRef marker) const {
	return *(world_information().markers())[marker->id()];
}
const Box& View::resolve_box_ref(BoxRef box) const {
	return static_cast<const Box&>(resolve_obstacle_ref(box));
}
const Sphere& View::resolve_sphere_ref(SphereRef sphere) const {
	return static_cast<const Sphere&>(resolve_obstacle_ref(sphere));
}
const Edge& View::resolve_edge_ref(EdgeRef edge) const {
	return *(world_information().get_according_edge(edge));
}
const Message& View::resolve_message_ref(MessageRef message) const {
	return *(world_information().get_according_message(message));
}

const Obstacle& View::resolve_obstacle_ref_safe(ObstacleRef obstacle) const {
	const Obstacle& result = resolve_obstacle_ref(obstacle);
	validate_identifier(result, obstacle);
	return result;
}
const RobotData& View::resolve_robot_ref_safe(RobotRef robot) const {
	const RobotData& result = resolve_robot_ref(robot);
//	validate_identifier(result, robot);
	if(result.id()->id() != robot->id()) {
		throw std::invalid_argument("Illegal identifier. Outdated? Cloned? Self-created?");
	}
	return result;
}
const WorldObject& View::resolve_marker_ref_safe(MarkerRef marker) const {
	const WorldObject& result = resolve_marker_ref(marker);
	//	validate_identifier(result, marker);
	if(result.id()->id() != marker->id()) {
		throw std::invalid_argument("Illegal identifier. Outdated? Cloned? Self-created?");
	}
	return result;
}
const Box& View::resolve_box_ref_safe(BoxRef box) const {
	const Box& result = resolve_box_ref(box);
	//	validate_identifier(result, box);
	if(result.id()->id() != box->id()) {
		throw std::invalid_argument("Illegal identifier. Outdated? Cloned? Self-created?");
	}
	return result;
}
const Sphere& View::resolve_sphere_ref_safe(SphereRef sphere) const {
	const Sphere& result = resolve_sphere_ref(sphere);
	//	validate_identifier(result, sphere);
	if(result.id()->id() != sphere->id()) {
		throw std::invalid_argument("Illegal identifier. Outdated? Cloned? Self-created?");
	}
	return result;
}
const Edge& View::resolve_edge_ref_safe(EdgeRef edge) const {
	const Edge& result = resolve_edge_ref(edge);
	//	validate_identifier(result, edge);
	if(result.id()->id() != edge->id()) {
		throw std::invalid_argument("Illegal identifier. Outdated? Cloned? Self-created?");
	}
	return result;
}
const Message& View::resolve_message_ref_safe(MessageRef message) const {
	const Message& result = resolve_message_ref(message);
	//	validate_identifier(result, message);
	if(result.id()->id() != message->id()) {
		throw std::invalid_argument("Illegal identifier. Outdated? Cloned? Self-created?");
	}
	return result;
}

void View::init(const boost::shared_ptr<WorldInformation>& world_information) {
	world_information_ = world_information;
}


const std::vector<View::RobotRef> View::get_visible_robots(const Robot& caller) const {
	std::vector<View::RobotRef> result(get_visible_robots(resolve_robot_ref(caller.id())));
	std::random_shuffle(result.begin(), result.end(), (generator_->get_value_uniform() % boost::lambda::_1));
	return result;
}


const std::vector<View::ObstacleRef> View::get_visible_obstacles(const Robot& caller) const {
	std::vector<View::ObstacleRef> result(get_visible_obstacles(resolve_robot_ref(caller.id())));
	std::random_shuffle(result.begin(), result.end(), (generator_->get_value_uniform() % boost::lambda::_1));
	return result;
}


const std::vector<View::MarkerRef> View::get_visible_markers(const Robot& caller) const {
	std::vector<View::MarkerRef> result(get_visible_markers(resolve_robot_ref(caller.id())));
	std::random_shuffle(result.begin(), result.end(), (generator_->get_value_uniform() % boost::lambda::_1));
	return result;
}

const std::vector<View::EdgeRef> View::get_visible_edges(const Robot& caller) const {
	std::vector<View::EdgeRef> result(get_visible_edges(resolve_robot_ref(caller.id())));
	std::random_shuffle(result.begin(), result.end(), (generator_->get_value_uniform() % boost::lambda::_1));
	return result;
}

const boost::shared_ptr<MessageIdentifier> View::get_message(const Robot& caller, std::size_t index) const {
	return get_message(resolve_robot_ref(caller.id()), index);
}

const std::size_t View::get_number_of_messages(const Robot& caller) const {
	return get_number_of_messages(resolve_robot_ref(caller.id()));
}

const Vector3d View::get_position(const Robot& caller, WorldObjectRef world_object) const {
	Vector3d position_global_coords;
	position_global_coords = delegate_function<Vector3d>(&View::get_own_position, &View::get_robot_position,
			                                             &View::get_obstacle_position, &View::get_marker_position,
			                                             &View::get_edge_position, &View::get_message_position,
	                                                     caller, world_object);
	// const RobotData& robot_data = resolve_robot_ref(caller.id());
	return position_global_coords;
	//return *CoordConverter::global_to_local(position_global_coords, robot_data.position(), robot_data.coordinate_system_axis());
}

const MarkerInformation View::get_marker_information(const Robot& caller, WorldObjectRef world_object) const {
	return delegate_function<MarkerInformation>(&View::get_own_marker_information, &View::get_robots_marker_information,
			                                           &View::get_obstacles_marker_information, &View::get_markers_marker_information,
			                                           &View::get_edge_marker_information, &View::get_message_marker_information,
			                                           caller, world_object);
}

const std::size_t View::get_id(const Robot& caller, WorldObjectRef world_object) const {
	return delegate_function<std::size_t>(&View::get_own_id, &View::get_robot_id,
	                                      &View::get_obstacle_id, &View::get_marker_id,
	                                      &View::get_edge_id, &View::get_message_id,
	                                      caller, world_object);
}

const Vector3d View::get_robot_acceleration(const Robot& caller, RobotRef robot) const {
	return delegate_function<Vector3d>(&View::get_own_acceleration, &View::get_others_acceleration, caller, robot);
}

const Vector3d View::get_robot_coordinate_system_origin(const Robot& caller, RobotRef robot) const {
	return delegate_function<Vector3d>(&View::get_own_coordinate_system_origin, &View::get_others_coordinate_system_origin, caller, robot);
}

const boost::tuple<boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d> > View::get_robot_coordinate_system_axis(
		const Robot& caller, RobotRef robot) const {
	return delegate_function<boost::tuple<boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d> > > (
			&View::get_own_coordinate_system_axis, &View::get_others_coordinate_system_axis, caller, robot);
}

const RobotType View::get_robot_type(const Robot& caller, RobotRef robot) const {
	return delegate_function<RobotType>(&View::get_own_type, &View::get_others_type, caller, robot);
}

const Vector3d View::get_robot_velocity(const Robot& caller, RobotRef robot) const {
	return delegate_function<Vector3d>(&View::get_own_velocity, &View::get_others_velocity, caller, robot);
}

const RobotStatus View::get_robot_status(const Robot& caller, RobotRef robot) const {
	return delegate_function<RobotStatus>(&View::get_own_status, &View::get_others_status, caller, robot);
}

const bool View::get_robot_last_request_successful(const Robot& caller, RobotRef robot) const {
	return delegate_function<bool>(&View::get_own_last_request_successful, &View::get_others_last_request_successful,
	                               caller, robot);
}

const bool View::is_point_in_obstacle(ObstacleRef obstacle, const Vector3d& point) const {
	return is_point_in_obstacle(resolve_obstacle_ref_safe(obstacle), point);
}

const double View::get_box_depth(BoxRef box) const {
	return get_box_depth(resolve_box_ref_safe(box));
}

const double View::get_box_width(BoxRef box) const {
	return get_box_width(resolve_box_ref_safe(box));
}

const double View::get_box_height(BoxRef box) const {
	return get_box_height(resolve_box_ref_safe(box));
}

const double View::get_sphere_radius(SphereRef sphere) const {
	return get_sphere_radius(resolve_sphere_ref_safe(sphere));
}

const bool View::is_edge_directed(EdgeRef edge) const {
	return is_edge_directed(resolve_edge_ref_safe(edge));
}

const View::RobotRef View::get_edge_source(EdgeRef edge) const {
	return get_edge_source(resolve_edge_ref_safe(edge));
}

const View::RobotRef View::get_edge_target(EdgeRef edge) const {
	return get_edge_target(resolve_edge_ref_safe(edge));
}

const View::RobotRef View::get_sender(MessageRef message) const {
	return get_sender(resolve_message_ref_safe(message));
}

std::vector<View::RobotRef> View::get_visible_robots(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_visible_robots"));
}

std::vector<View::ObstacleRef> View::get_visible_obstacles(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_visible_obstacles"));
}

std::vector<View::MarkerRef> View::get_visible_markers(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_visible_markers"));
}

std::vector<View::EdgeRef> View::get_visible_edges(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_visible_edges"));
}

boost::shared_ptr<MessageIdentifier> View::get_message(const RobotData& robot, std::size_t index) const {
	throw UnsupportedOperationException(get_error_message("get_message"));
}

std::size_t View::get_number_of_messages(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_number_of_messages"));
}

Vector3d View::get_own_position(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_position"));
}

Vector3d View::get_robot_position(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robot_position"));
}

Vector3d View::get_obstacle_position(const Obstacle& obstacle) const {
	throw UnsupportedOperationException(get_error_message("get_obstacle_position"));
}

Vector3d View::get_marker_position(const WorldObject& marker) const {
	throw UnsupportedOperationException(get_error_message("get_obstacle_position"));
}

Vector3d View::get_edge_position(const Edge& edge) const {
	throw UnsupportedOperationException(get_error_message("get_edge_position"));
}

Vector3d View::get_message_position(const Message& messsage) const {
	throw UnsupportedOperationException(get_error_message("get_message_position"));
}

MarkerInformation View::get_own_marker_information(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_marker_information"));
}

MarkerInformation View::get_robots_marker_information(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robots_marker_information"));
}

MarkerInformation View::get_obstacles_marker_information(const Obstacle& obstacle) const {
	throw UnsupportedOperationException(get_error_message("get_obstacles_marker_information"));
}

MarkerInformation View::get_markers_marker_information(const WorldObject& marker) const {
	throw UnsupportedOperationException(get_error_message("get_markers_marker_information"));
}

MarkerInformation View::get_edge_marker_information(const Edge& edge) const {
	throw UnsupportedOperationException(get_error_message("get_edge_marker_information"));
}

MarkerInformation View::get_message_marker_information(const Message& message) const {
	throw UnsupportedOperationException(get_error_message("get_message_marker_information"));
}

std::size_t View::get_own_id(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_id"));
}

std::size_t View::get_robot_id(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_sphere_radius"));
}

std::size_t View::get_obstacle_id(const Obstacle& robot) const {
	throw UnsupportedOperationException(get_error_message("get_obstacle_id"));
}

std::size_t View::get_marker_id(const WorldObject& robot) const {
	throw UnsupportedOperationException(get_error_message("get_marker_id"));
}

std::size_t View::get_edge_id(const Edge& edge) const {
	throw UnsupportedOperationException(get_error_message("get_edge_id"));
}

std::size_t View::get_message_id(const Message& message) const {
	throw UnsupportedOperationException(get_error_message("get_message_id"));
}

Vector3d View::get_own_acceleration(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_acceleration"));
}

Vector3d View::get_others_acceleration(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robot_acceleration"));
}

Vector3d View::get_own_coordinate_system_origin(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_coordinate_system_origin"));
}

Vector3d View::get_others_coordinate_system_origin(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_others_coordinate_system_origin"));
}

boost::tuple<boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d> > View::get_own_coordinate_system_axis(
		const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_coordinate_system_axis"));
}

boost::tuple<boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d> > View::get_others_coordinate_system_axis(
		const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robot_coordinate_system_axis"));
}

RobotType View::get_own_type(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_type"));
}

RobotType View::get_others_type(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robot_type"));
}

Vector3d View::get_own_velocity(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_velocity"));
}

Vector3d View::get_others_velocity(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robot_velocity"));
}

RobotStatus View::get_own_status(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_status"));
}

RobotStatus View::get_others_status(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robot_status"));
}

bool View::get_own_last_request_successful(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_last_request_successful"));
}

bool View::get_others_last_request_successful(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_last_request_successful"));
}

bool View::is_point_in_obstacle(const Obstacle& obstacle, const Vector3d& point) const {
	throw UnsupportedOperationException(get_error_message("is_point_in_obstacle"));
}

double View::get_box_depth(const Box& box) const {
	throw UnsupportedOperationException(get_error_message("get_box_depth"));
}

double View::get_box_width(const Box& box) const {
	throw UnsupportedOperationException(get_error_message("get_box_width"));
}

double View::get_box_height(const Box& box) const {
	throw UnsupportedOperationException(get_error_message("get_box_height"));
}

double View::get_sphere_radius(const Sphere& sphere) const {
	throw UnsupportedOperationException(get_error_message("get_sphere_radius"));
}

bool View::is_edge_directed(const Edge& edge) const {
	throw UnsupportedOperationException(get_error_message("is_edge_directed"));
}

View::RobotRef View::get_edge_source(const Edge& robot) const{
	throw UnsupportedOperationException(get_error_message("get_edge_source"));
}

View::RobotRef View::get_edge_target(const Edge& edge) const{
	throw UnsupportedOperationException(get_error_message("get_edge_target"));
}

View::RobotRef View::get_sender(const Message& message) const{
	throw UnsupportedOperationException(get_error_message("get_sender"));
}

const WorldInformation& View::world_information() const {
	return *world_information_;
}

const std::size_t View::get_id(boost::shared_ptr<Identifier> identifier) const {
	return identifier->id();
}

const int View::get_time() const {
	throw UnsupportedOperationException(get_error_message("get_time"));
}

const double View::get_view_radius() const {
	throw UnsupportedOperationException(get_error_message("get_view_radius"));
}

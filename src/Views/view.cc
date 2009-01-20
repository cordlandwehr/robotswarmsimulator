#include "view.h"



#include "../Model/identifier.h"

#include "../Model/robot_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/marker_identifier.h"
#include "../Model/box_identifier.h"
#include "../Model/sphere_identifier.h"
#include "../Model/robot.h"
#include "../Utilities/vector3d.h"
#include "../Model/marker_information.h"
#include "../Model/obstacle.h"
#include "../Model/box.h"
#include "../Model/sphere.h"
#include "../Model/world_information.h"
#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/coord_converter.h"

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
		return method_name + "not implemented in this model.";
	}

}

template<typename T>
T View::delegate_function(boost::function<T (const View*, const RobotData&)> own_robot_fun, boost::function<T (const View*, const RobotData&)> other_robot_fun,
						boost::function<T (const View*, const Obstacle&)> obstacle_fun, boost::function<T (const View*, const WorldObject&)> marker_fun,
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
	}
	else {
		throw std::invalid_argument("Illegal type of world_object.");
	}
}

template<typename T>
T View::delegate_function(boost::function<T (const View*, const RobotData&)> own_robot_fun, boost::function<T (const View*, const RobotData&)> other_robot_fun,
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
	return *(world_information().robot_data())[robot->id()];
}
const WorldObject& View::resolve_marker_ref(MarkerRef marker) const {
	return *(world_information().markers())[marker->id()];
}
const Box& View::resolve_box_ref(BoxRef box) const {
	return dynamic_cast<const Box&>(resolve_obstacle_ref(box));
}
const Sphere& View::resolve_sphere_ref(SphereRef sphere) const {
	return dynamic_cast<const Sphere&>(resolve_obstacle_ref(sphere));
}

const Obstacle& View::resolve_obstacle_ref_safe(ObstacleRef obstacle) const {
	const Obstacle& result = resolve_obstacle_ref(obstacle);
	validate_identifier(result, obstacle);
	return result;
}
const RobotData& View::resolve_robot_ref_safe(RobotRef robot) const {
	const RobotData& result = resolve_robot_ref(robot);
	validate_identifier(result, robot);
	return result;
}
const WorldObject& View::resolve_marker_ref_safe(MarkerRef marker) const {
	const WorldObject& result = resolve_marker_ref(marker);
	validate_identifier(result, marker);
	return result;
}
const Box& View::resolve_box_ref_safe(BoxRef box) const {
	const Box& result = resolve_box_ref(box);
	validate_identifier(result, box);
	return result;
}
const Sphere& View::resolve_sphere_ref_safe(SphereRef sphere) const {
	const Sphere& result = resolve_sphere_ref(sphere);
	validate_identifier(result, sphere);
	return result;
}

void View::init(const WorldInformation& world_information) {
	world_information_ = &world_information;
}

std::set<View::RobotRef> View::get_visible_robots(const Robot& caller) const {
	return get_visible_robots(resolve_robot_ref(caller.id()));
}

std::set<View::ObstacleRef> View::get_visible_obstacles(const Robot& caller) const {
	return get_visible_obstacles(resolve_robot_ref(caller.id()));
}

std::set<View::MarkerRef> View::get_visible_markers(const Robot& caller) const {
	return get_visible_markers(resolve_robot_ref(caller.id()));
}

Vector3d View::get_position(const Robot& caller, WorldObjectRef world_object) const {
	Vector3d position_global_coords;
	position_global_coords = delegate_function<Vector3d>(&View::get_own_position, &View::get_robot_position,
			                                             &View::get_obstacle_position, &View::get_marker_position,
	                                                     caller, world_object);
	const RobotData& robot_data = resolve_robot_ref(caller.id());
	return *CoordConverter::global_to_local(position_global_coords, robot_data.position(), robot_data.coordinate_system_axis());
}

const MarkerInformation& View::get_marker_information(const Robot& caller, WorldObjectRef world_object) const {
	return delegate_function<const MarkerInformation&>(&View::get_own_marker_information, &View::get_robots_marker_information,
			                                           &View::get_obstacles_marker_information, &View::get_markers_marker_information,
			                                           caller, world_object);
}

std::size_t View::get_id(const Robot& caller, WorldObjectRef world_object) const {
	return delegate_function<std::size_t>(&View::get_own_id, &View::get_robot_id,
	                                      &View::get_obstacle_id, &View::get_marker_id,
	                                      caller, world_object);
}

Vector3d View::get_robot_acceleration(const Robot& caller, RobotRef robot) const {
	return delegate_function<Vector3d>(&View::get_own_acceleration, &View::get_others_acceleration, caller, robot);
}

boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > View::get_robot_coordinate_system_axis(const Robot& caller, RobotRef robot) const {
	return delegate_function<boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > >(&View::get_own_coordinate_system_axis, &View::get_others_coordinate_system_axis, caller, robot);
}

RobotType View::get_robot_type(const Robot& caller, RobotRef robot) const {
	return delegate_function<RobotType>(&View::get_own_type, &View::get_others_type, caller, robot);
}

Vector3d View::get_robot_velocity(const Robot& caller, RobotRef robot) const {
	return delegate_function<Vector3d>(&View::get_own_velocity, &View::get_others_velocity, caller, robot);
}

RobotStatus View::get_robot_status(const Robot& caller, RobotRef robot) const {
	return delegate_function<RobotStatus>(&View::get_own_status, &View::get_others_status, caller, robot);
}

bool View::is_point_in_obstacle(ObstacleRef obstacle, const Vector3d& point) const {
	return is_point_in_obstacle(resolve_obstacle_ref_safe(obstacle), point);
}

double View::get_box_depth(BoxRef box) const {
	return get_box_depth(resolve_box_ref_safe(box));
}

double View::get_box_width(BoxRef box) const {
	return get_box_width(resolve_box_ref_safe(box));
}

double View::get_box_height(BoxRef box) const {
	return get_box_height(resolve_box_ref_safe(box));
}

double View::get_sphere_radius(SphereRef sphere) const {
	return get_sphere_radius(resolve_sphere_ref_safe(sphere));
}

std::set<View::RobotRef> View::get_visible_robots(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_visible_robots"));
}

std::set<View::ObstacleRef> View::get_visible_obstacles(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_visible_obstacles"));
}

std::set<View::MarkerRef> View::get_visible_markers(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_visible_markers"));
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

const MarkerInformation& View::get_own_marker_information(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_marker_information"));
}

const MarkerInformation& View::get_robots_marker_information(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robots_marker_information"));
}

const MarkerInformation& View::get_obstacles_marker_information(const Obstacle& obstacle) const {
	throw UnsupportedOperationException(get_error_message("get_obstacles_marker_information"));
}

const MarkerInformation& View::get_markers_marker_information(const WorldObject& marker) const {
	throw UnsupportedOperationException(get_error_message("get_markers_marker_information"));
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

Vector3d View::get_own_acceleration(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_acceleration"));
}

Vector3d View::get_others_acceleration(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_robot_acceleration"));
}

boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > View::get_own_coordinate_system_axis(const RobotData& robot) const {
	throw UnsupportedOperationException(get_error_message("get_own_coordinate_system_axis"));
}

boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > View::get_others_coordinate_system_axis(const RobotData& robot) const {
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

const WorldInformation& View::world_information() const {
	return *world_information_;
}

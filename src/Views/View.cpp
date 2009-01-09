#if 0
#include "View.h"



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

View::View() {
}

View::~View() {
}

namespace {
	void validate_identifier(const WorldObject& obj, boost::shared_ptr<Identifier> identifier) {
		if(obj.id().get() != identifier.get()) {
			throw std::invalid_argument("Illegal identifier. Outdated?");
		}
	}

	bool is_own_identifier(const Robot& robot, boost::shared_ptr<RobotIdentifier> identifier) {
		return robot.id()->id() == identifier->id();
	}
}

const Obstacle& View::resolve_obstacle_ref(ObstacleRef obstacle) const {
	return (world_information()->obstacles())[obstacle->id()];
}
const RobotData& View::resolve_robot_ref(RobotRef robot) const {
	return (world_information()->robot_data())[robot->id()];
}
const WorldObject& View::resolve_marker_ref(MarkerRef marker) const {
	return (world_information()->markers())[marker->id()];
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
	validate_identifier(result, marker);
	return result;
}
const Sphere& View::resolve_sphere_ref_safe(SphereRef sphere) const {
	const Sphere& result = resolve_sphere_ref(sphere);
	validate_identifier(result, marker);
	return result;
}

void View::init(boost::shared_ptr<WorldInformation> world_information) {
	world_information_ = world_information;
}

Vector3d View::get_position(const Robot& caller, WorldObjectRef world_object) const {
	if(RobotRef ref = boost::dynamic_pointer_cast<RobotIdentifier>(world_object)) {
		if(is_own_identifier(caller, ref)) {
			return get_own_position(resolve_robot_ref(ref));
		} else {
			return get_robot_position(resolve_robot_ref_safe(ref));
		}
	} else if(ObstacleRef ref = boost::dynamic_pointer_cast<ObstacleIdentifier>(world_object)) {
		return get_obstacle_position(resolve_obstacle_ref_safe(ref));
	} else if(MarkerRef ref = boost::dynamic_pointer_cast<MarkerIdentifier>(world_object)) {
		return get_marker_position(resolve_marker_ref_safe(ref));
	}
	else {
		throw std::invalid_argument("Illegal type of world_object.");
	}
}
#endif

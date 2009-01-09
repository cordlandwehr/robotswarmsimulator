/*
 * View.h
 *
 *  Created on: 08.01.2009
 *      Author: Daniel Wonisch
 */
#ifndef VIEW_H_
#define VIEW_H_

#include <cstddef>
#include <set>
#include <boost/shared_ptr.hpp>
#include "../Model/robot_data.h"

class Identifier;
class RobotIdentifier;
class ObstacleIdentifier;
class MarkerIdentifier;
class BoxIdentifier;
class SphereIdentifier;
class Robot;
class vector3d;
class MarkerInformation;
class Obstacle;
class Box;
class Sphere;
class WorldInformation;

class View {
protected:
	typedef boost::shared_ptr<RobotIdentifier> RobotRef;
	typedef boost::shared_ptr<ObstacleIdentifier> ObstacleRef;
	typedef boost::shared_ptr<BoxIdentifier> BoxRef;
	typedef boost::shared_ptr<SphereIdentifier> SphereRef;
	typedef boost::shared_ptr<MarkerIdentifier> MarkerRef;
	typedef boost::shared_ptr<Identifier> WorldObjectRef;


public:
	View();
	virtual ~View();

	/**
	 * Although init is trivial, it is nice to have a default constructor in virtual
	 * inherited classes. Otherwise every arbitrary deep sub class has
	 * to call the non-default constructor.
	 */
	virtual void init(boost::shared_ptr<WorldInformation> world_information);

	std::set<RobotRef> get_visible_robots(const Robot& caller) const;
	std::set<ObstacleRef> get_visible_obstacles(const Robot& caller) const;
	std::set<MarkerRef> get_visible_markers(const Robot& caller) const;

	//-- WorldObject--
	Vector3d get_position(const Robot& caller, WorldObjectRef world_object) const;
	const MarkerInformation& get_marker_information(const Robot& caller, WorldObjectRef world_object) const;

	//-- RobotData --
	std::size_t get_robot_id(const Robot& caller, RobotRef robot) const;
	Vector3d get_robot_acceleration(const Robot& caller, RobotRef robot) const;
	boost::tuple<Vector3d> get_robot_coordinate_system_axis(const Robot& caller, RobotRef robot) const;
	RobotType get_robot_type(const Robot& caller, RobotRef robot) const;
	Vector3d get_robot_velocity(const Robot& caller, RobotRef robot) const;
	RobotStatus get_robot_status(const Robot& caller, RobotRef robot) const;

	//-- Obstacle --
	bool is_point_in_obstacle(ObstacleRef obstacle, const Vector3d& point) const;

	//-- Box --
	double get_box_depth(BoxRef obstacle) const;
	double get_box_width(BoxRef obstacle) const;
	double get_box_height(BoxRef obstacle) const;

	//-- Sphere --
	double get_sphere_radius(SphereRef sphere) const;



protected:
	//Helper methods for non virtual methods
	virtual std::set<RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::set<ObstacleRef> get_visible_obstacles(const RobotData& robot) const;
	virtual std::set<MarkerRef> get_visible_markers(const RobotData& robot) const;

	virtual Vector3d get_own_position(const RobotData& robot) const;
	virtual Vector3d get_robot_position(const RobotData& robot) const;
	virtual Vector3d get_obstacle_position(const Obstacle& obstacle) const;
	virtual Vector3d get_marker_position(const WorldObject& marker) const;

	virtual const MarkerInformation& get_own_marker_information(const RobotData& robot) const;
	virtual const MarkerInformation& get_robots_marker_information(const RobotData& robot) const;
	virtual const MarkerInformation& get_obstacles_marker_information(const Obstacle& obstacle) const;
	virtual const MarkerInformation& get_markers_marker_information(const WorldObject& marker) const;

	virtual std::size_t get_own_id(const RobotData& robot) const;
	virtual std::size_t get_robot_id(const RobotData& robot) const;

	virtual Vector3d get_own_acceleration(const RobotData& robot) const;
	virtual Vector3d get_robot_acceleration(const RobotData& robot) const;

	virtual boost::tuple<Vector3d> get_own_coordinate_system_axis(const RobotData& robot) const;
	virtual boost::tuple<Vector3d> get_robot_coordinate_system_axis(const RobotData& robot) const;

	virtual RobotType get_own_type(const RobotData& robot) const;
	virtual RobotType get_robot_type(const RobotData& robot) const;

	virtual Vector3d get_own_velocity(const RobotData& robot) const;
	virtual Vector3d get_robot_velocity(const RobotData& robot) const;

	virtual RobotStatus get_own_status(const RobotData& robot) const;
	virtual RobotStatus get_robot_status(const RobotData& robot) const;

	virtual bool is_point_in_obstacle(const Obstacle& obstacle, const Vector3d& point) const;

	virtual double get_box_depth(const Box& box) const;
	virtual double get_box_width(const Box& box) const;
	virtual double get_box_height(const Box& box) const;

	virtual double get_sphere_radius(const Sphere& sphere) const;


	const boost::shared_ptr<WorldInformation>& world_information() const;

private:
	const Obstacle& resolve_obstacle_ref(ObstacleRef obstacle) const;
	const RobotData& resolve_robot_ref(RobotRef robot) const;
	const WorldObject& resolve_marker_ref(MarkerRef marker) const;
	const Box& resolve_box_ref(BoxRef box) const;
	const Sphere& resolve_sphere_ref(SphereRef sphere) const;

	const Obstacle& resolve_obstacle_ref_safe(ObstacleRef obstacle) const;
	const RobotData& resolve_robot_ref_safe(RobotRef robot) const;
	const WorldObject& resolve_marker_ref_safe(MarkerRef marker) const;
	const Box& resolve_box_ref_safe(BoxRef box) const;
	const Sphere& resolve_sphere_ref_safe(SphereRef sphere) const;

private:
	boost::shared_ptr<WorldInformation> world_information_;

};

#endif /* VIEW_H_ */

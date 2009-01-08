/*
 * View.h
 *
 *  Created on: 08.01.2009
 *      Author: Daniel Wonisch
 */
#if 0
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
class RobotData;


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

	virtual std::set<RobotRef> get_visible_robots(const Robot& caller) const;
	virtual std::set<RobotRef> get_visibile_obstacles(const Robot& caller) const;
	virtual std::set<RobotRef> get_visible_markers(const Robot& caller) const;

	//-- WorldObject--
	vector3d get_position(const Robot& caller, WorldObjectRef world_object) const;
	const MarkerInformation& get_marker_information(const Robot& caller, WorldObjectRef world_object) const;

	//-- RobotData --
	//TODO: This looks strange, but the Robot.compute method is not allowed to get the id
	//      out of the Identifier object. Therefore we provide an additional method to
	//		get this id of the Identifier. That may be forbidden, if the Robot must not
	//		be able to receive identities of the (other) Robots.
	std::size_t get_robot_id(const Robot& caller, RobotRef robot) const;
	vector3d get_robot_acceleration(const Robot& caller, RobotRef robot) const;
	boost::tuple<vector3d> get_robot_coordinate_system_axis(const Robot& caller, RobotRef robot) const;
	Type get_robot_type(const Robot& caller, RobotRef robot) const;
	vector3d get_robot_velocity(const Robot& caller, RobotRef robot) const;
	Status get_robot_status(const Robot& caller, RobotRef robot) const;

	//-- Obstacle --
	//TODO: needed?
	virtual bool is_point_in_obstacle(ObstacleRef obstacle, vector3d point) const;

	//-- Box --
	virtual double get_box_depth(BoxRef obstacle) const;
	virtual double get_box_width(BoxRef obstacle) const;
	virtual double get_box_height(BoxRef obstacle) const;

	//-- Sphere --
	virtual double get_sphere_radius(SphereRef sphere) const;



protected:
	//Helper methods for non virtual methods
	virtual vector3d get_own_position(RobotRef robot) const;
	virtual vector3d get_robot_position(RobotRef robot) const;
	virtual vector3d get_obstacle_position(ObstacleRef robot) const;
	virtual vector3d get_marker_position(MarkerRef robot) const;

	virtual const MarkerInformation& get_own_marker_information(RobotRef robot) const;
	virtual const MarkerInformation& get_robots_marker_information(RobotRef robot) const;
	virtual const MarkerInformation& get_obstacles_marker_information(ObstacleRef robot) const;
	virtual const MarkerInformation& get_markers_marker_information(MarkerRef robot) const;

	virtual std::size_t get_robot_id(RobotRef robot) const;
	virtual std::size_t get_own_id(RobotRef robot) const;

	virtual boost::tuple<vector3d> get_own_acceleration(RobotRef robot) const;
	virtual boost::tuple<vector3d> get_robot_acceleration(RobotRef robot) const;

	virtual boost::tuple<vector3d> get_own_coordinate_system_axis(RobotRef robot) const;
	virtual boost::tuple<vector3d> get_robot_coordinate_system_axis(RobotRef robot) const;

	virtual Type get_own_type(RobotRef robot) const;
	virtual Type get_robot_type(RobotRef robot) const;

	virtual vector3d get_own_velocity(RobotRef robot) const;
	virtual vector3d get_robot_velocity(RobotRef robot) const;

	virtual Status get_own_status(RobotRef robot) const;
	virtual Status get_robot_status(RobotRef robot) const;



	const Obstacle& resolveObstacleRef(ObstacleRef obstacle);
	const RobotData& resolveRobotRef(RobotRef robot);
	const WorldObject& resolveMarkerRef(MarkerRef marker);
	const Box& resolveMarkerRef(BoxRef marker);
	const Sphere& resolveMarkerRef(SphereRef marker);

};

#endif /* VIEW_H_ */
#endif

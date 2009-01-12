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
#include <boost/tuple/tuple.hpp>
#include "../Model/robot_data.h"
#include "../Utilities/vector3d.h"

class Identifier;
class RobotIdentifier;
class ObstacleIdentifier;
class MarkerIdentifier;
class BoxIdentifier;
class SphereIdentifier;
class Robot;
class MarkerInformation;
class Obstacle;
class Box;
class Sphere;
class WorldInformation;

/**
 * \class View
 * \brief Interface for Robot::compute() to the WorldInformation.
 *
 * The View class represents the robots view to the world, i.e. a View class describes the information model
 * that should be used for this simulation.
 * This class is designed as a base class for more concrete models. Still there are no abstract methods. Instead,
 * all methods are implemented such, that they throw a NotSupportedOperationException.
 *
 * Note: Most methods specified in this class need to know the calling Robot, so they determine whether
 *       e.g. the own position is queried or the position of an other Robot.
 *
 * Assigning this class to a Robot corresponds to a "no information at all" model for each Robot.
 *
 */
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
	 * Call this method before using any other method of View.
	 * @param WorldInformation
	 */
	virtual void init(const WorldInformation& world_information);

	/**
	 * Returns the set of robots visible for the calling Robot.
	 * @param Robot that should take a look-around
	 * @return set of robots visible
	 */
	std::set<RobotRef> get_visible_robots(const Robot& caller) const;
	/**
	* Returns the set of obstacles visible for the calling Robot.
	* @param Robot that should take a look-around
	* @return set of obstacles visible
	*/
	std::set<ObstacleRef> get_visible_obstacles(const Robot& caller) const;
	/**
	* Returns the set of markers visible for the calling Robot.
	* @param Robot that should take a look-around
	* @return set of markers visible
	*/
	std::set<MarkerRef> get_visible_markers(const Robot& caller) const;

	//-- WorldObject--
	/**
	 * Queries for the position of a WorldObject identified by an given Identifier.
	 * @param The Robot which is asking..
	 * @param Identifier for a WorldObject
	 * @return Position
	 * @see WorldObject::position()
	 */
	Vector3d get_position(const Robot& caller, WorldObjectRef world_object) const;
	/**
	 * Queries for the marker information of a WorldObject identified by an given Identifier.
	 * @param The Robot which is asking..
	 * @param Identifier for a WorldObject
	 * @return MarkerInformation
	 * @see WorldObject::marker_information()
	 */
	const MarkerInformation& get_marker_information(const Robot& caller, WorldObjectRef world_object) const;
	/**
	 * Queries for the id of a WordObject identified by an given Identifier.
	 * @param The Robot which is asking..
	 * @param Identifier for a WorldObject
	 * @return id
	 * @see WorldObject::id()
	 */
	std::size_t get_id(const Robot& caller, WorldObjectRef robot) const;
	//-- RobotData --
	/**
	 * Queries for the acceleration of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return Vector3d
	 * @see RobotData::acceleration()
	 */
	Vector3d get_robot_acceleration(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the coordinate system axes of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return Vector3d
	 * @see RobotData::coordinate_system_axis()
	 */
	boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_robot_coordinate_system_axis(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the robot type of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return RobotType
	 * @see RobotData::type()
	 */
	RobotType get_robot_type(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the velocity of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return Vector3d
	 * @see RobotData::velocity()
	 */
	Vector3d get_robot_velocity(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the status of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return RobotStatus
	 * @see RobotData::status()
	 */
	RobotStatus get_robot_status(const Robot& caller, RobotRef robot) const;

	//-- Obstacle --
	/**
	 * Checks for a given point whether it is contained in a Obstacle identified by a given ObstacleIdentifier.
	 * @param ObstacleIdentifier
	 * @param point
	 * @return true if point is contained in the obstacle; false, otherwise
	 * @see Obstacle::contains_point()
	 */
	bool is_point_in_obstacle(ObstacleRef obstacle, const Vector3d& point) const;

	//-- Box --
	/**
	 * Queries for the depth of a Box identified by an given BoxIdentifier.
	 * @param BoxIdentifier
	 * @return depth
	 * @see Box::depth()
	 */
	double get_box_depth(BoxRef box) const;
	/**
	 * Queries for the width of a Box identified by an given BoxIdentifier.
	 * @param BoxIdentifier
	 * @return width
	 * @see Box::width()
	 */
	double get_box_width(BoxRef box) const;
	/**
	 * Queries for the height of a Box identified by an given BoxIdentifier.
	 * @param BoxIdentifier
	 * @return height
	 * @see Box::height()
	 */
	double get_box_height(BoxRef box) const;

	//-- Sphere --
	/**
	 * Queries for the radius of a Sphere identified by an given SphereIdentifier.
	 * @param SphereIdentifier
	 * @return radius
	 * @see Sphere::radius()
	 */
	double get_sphere_radius(SphereRef sphere) const;



protected:
	//Helper methods for non virtual methods.
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
	virtual std::size_t get_obstacle_id(const Obstacle& obstacle) const;
	virtual std::size_t get_marker_id(const WorldObject& marker) const;

	virtual Vector3d get_own_acceleration(const RobotData& robot) const;
	virtual Vector3d get_robot_acceleration(const RobotData& robot) const;

	virtual boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_own_coordinate_system_axis(const RobotData& robot) const;
	virtual boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_robot_coordinate_system_axis(const RobotData& robot) const;

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


	const WorldInformation& world_information() const;

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

	static bool is_own_identifier(const Robot& robot, boost::shared_ptr<RobotIdentifier> identifier);

private:
	const WorldInformation* world_information_;

};

#endif /* VIEW_H_ */

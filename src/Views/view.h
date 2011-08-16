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
#include <boost/utility.hpp>
#include <boost/function.hpp>
#include "../Model/robot_data.h"
#include "../Utilities/vector_arithmetics.h"
#include "../Model/identifier.h"
//class Identifier;
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
class View : private boost::noncopyable {
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
	virtual void init(const boost::shared_ptr<WorldInformation>& world_information);

	/**
	 * Returns the set of robots visible for the calling Robot.
	 * Note: The Identifier of the calling Robot is never returned by this method.
	 * @param Robot that should take a look-around
	 * @return set of robots visible
	 */

	const std::vector<RobotRef> get_visible_robots(const Robot& caller) const;
	/**
	* Returns the set of obstacles visible for the calling Robot.
	* @param Robot that should take a look-around
	* @return set of obstacles visible
	*/

	const std::vector<ObstacleRef> get_visible_obstacles(const Robot& caller) const;

	/**
	* Returns the set of markers visible for the calling Robot.
	* @param Robot that should take a look-around
	* @return set of markers visible
	*/

	const std::vector<MarkerRef> get_visible_markers(const Robot& caller) const;


	//-- WorldObject--
	/**
	 * Queries for the position of a WorldObject identified by an given Identifier.
	 * Note: as of August 16th 2011, the absolute position is returned (asetzer)
	 * @param The Robot which is asking..
	 * @param Identifier for a WorldObject
	 * @return Position
	 * @see WorldObject::position()
	 */
	const Vector3d get_position(const Robot& caller, WorldObjectRef world_object) const;
	
	/**
	 * Queries for the marker information of a WorldObject identified by an given Identifier.
	 * @param The Robot which is asking..
	 * @param Identifier for a WorldObject
	 * @return MarkerInformation
	 * @see WorldObject::marker_information()
	 */
	const MarkerInformation get_marker_information(const Robot& caller, WorldObjectRef world_object) const;
	/**
	 * Queries for the id of a WordObject identified by an given Identifier.
	 * @param The Robot which is asking..
	 * @param Identifier for a WorldObject
	 * @return id
	 * @see WorldObject::id()
	 */
	const std::size_t get_id(const Robot& caller, WorldObjectRef robot) const;
	//-- RobotData --
	/**
	 * Queries for the acceleration of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return Vector3d
	 * @see RobotData::acceleration()
	 */
	const Vector3d get_robot_acceleration(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the coordinate system axes of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return Vector3d
	 * @see RobotData::coordinate_system_axis()
	 */
	const boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_robot_coordinate_system_axis(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the coordinate system origin (i.e. global position) of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return Vector3d
	 * @see RobotData::position()
	 */
	const Vector3d get_robot_coordinate_system_origin(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the robot type of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return RobotType
	 * @see RobotData::type()
	 */
	const RobotType get_robot_type(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the velocity of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return Vector3d
	 * @see RobotData::velocity()
	 */
	const Vector3d get_robot_velocity(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries for the status of a Robot identified by an given RobotIdentifier.
	 * @param The Robot which is asking..
	 * @param RobotIdentifier
	 * @return RobotStatus
	 * @see RobotData::status()
	 */
	const RobotStatus get_robot_status(const Robot& caller, RobotRef robot) const;
	/**
	 * Queries whether last (already performed) request issued by the given robot has been successful, that is has been
	 * performed exactly as requested without any change (due to inaccuracy, collisions, ...).
	 * @param The robot which is asking..
	 * @param RobotIdentifier
	 * @return bool
	 * @see RobotData::last_request_successful()
	 */
	const bool get_robot_last_request_successful(const Robot& caller, RobotRef robot) const;

	//-- Obstacle --
	/**
	 * Checks for a given point whether it is contained in a Obstacle identified by a given ObstacleIdentifier.
	 * @param ObstacleIdentifier
	 * @param point
	 * @return true if point is contained in the obstacle; false, otherwise
	 * @see Obstacle::contains_point()
	 */
	const bool is_point_in_obstacle(ObstacleRef obstacle, const Vector3d& point) const;

	//-- Box --
	/**
	 * Queries for the depth of a Box identified by an given BoxIdentifier.
	 * @param BoxIdentifier
	 * @return depth
	 * @see Box::depth()
	 */
	const double get_box_depth(BoxRef box) const;
	/**
	 * Queries for the width of a Box identified by an given BoxIdentifier.
	 * @param BoxIdentifier
	 * @return width
	 * @see Box::width()
	 */
	const double get_box_width(BoxRef box) const;
	/**
	 * Queries for the height of a Box identified by an given BoxIdentifier.
	 * @param BoxIdentifier
	 * @return height
	 * @see Box::height()
	 */
	const double get_box_height(BoxRef box) const;

	//-- Sphere --
	/**
	 * Queries for the radius of a Sphere identified by an given SphereIdentifier.
	 * @param SphereIdentifier
	 * @return radius
	 * @see Sphere::radius()
	 */
	const double get_sphere_radius(SphereRef sphere) const;

	//-- WorldInformation --
	/**
	 * Queries the time of the WorldInformation this object provides view to.
	 * @return time
	 * @see WorldInformation::time()
	 */
	virtual const int get_time() const;

	/**
	 * Queries the view radius of the view. Throws an UnsupportedOperationException 
	 * if the view does not have a view radius.
	 * @return view radius
	 */
	virtual const double get_view_radius() const;



protected:
	//Helper methods for non virtual methods.

	virtual std::vector<RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::vector<ObstacleRef> get_visible_obstacles(const RobotData& robot) const;
	virtual std::vector<MarkerRef> get_visible_markers(const RobotData& robot) const;


	//Following methods are called by the non virtual method get_position. Which of the four methods
	//is called is decided by the actual type of the given WorldObject type and by the question whether
	//the given RobotRef references on the calling robot.
	//Note: Return global coordinates here. They are automatically converted to local coordinates in
	//View::get_position method.
	//@see View::get_position
	//@see View::delegate_function
	virtual Vector3d get_own_position(const RobotData& robot) const;
	virtual Vector3d get_robot_position(const RobotData& robot) const;
	virtual Vector3d get_obstacle_position(const Obstacle& obstacle) const;
	virtual Vector3d get_marker_position(const WorldObject& marker) const;

	//Pretty much the same concept as in View::get_own_position besides this methods are called by
	//View::get_marker_information.
	virtual MarkerInformation get_own_marker_information(const RobotData& robot) const;
	virtual MarkerInformation get_robots_marker_information(const RobotData& robot) const;
	virtual MarkerInformation get_obstacles_marker_information(const Obstacle& obstacle) const;
	virtual MarkerInformation get_markers_marker_information(const WorldObject& marker) const;

	//Pretty much the same concept as in View::get_own_position besides this methods are called by
	//View::get_id.
	virtual std::size_t get_own_id(const RobotData& robot) const;
	virtual std::size_t get_robot_id(const RobotData& robot) const;
	virtual std::size_t get_obstacle_id(const Obstacle& obstacle) const;
	virtual std::size_t get_marker_id(const WorldObject& marker) const;

	//Following two methods are called by get_robot_acceleration. If the given RobotRef is referencing the
	//calling Robot then get_own_acceleration is called, otherwise get_others_acceleration.
	virtual Vector3d get_own_acceleration(const RobotData& robot) const;
	virtual Vector3d get_others_acceleration(const RobotData& robot) const;

	//Pretty much the same concept as in View::get_own_acceleration besides this methods are called by
	//View::get_robot_coordinate_system_origin.
	virtual Vector3d get_own_coordinate_system_origin(const RobotData& robot) const;
	virtual Vector3d get_others_coordinate_system_origin(const RobotData& robot) const;

	//Pretty much the same concept as in View::get_own_acceleration besides this methods are called by
	//View::get_robot_coordinate_system_axis.
	virtual boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_own_coordinate_system_axis(const RobotData& robot) const;
	virtual boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_others_coordinate_system_axis(const RobotData& robot) const;

	//Pretty much the same concept as in View::get_own_acceleration besides this methods are called by
	//View::get_robot_type.
	virtual RobotType get_own_type(const RobotData& robot) const;
	virtual RobotType get_others_type(const RobotData& robot) const;

	//Pretty much the same concept as in View::get_own_acceleration besides this methods are called by
	//View::get_robot_velocity.
	virtual Vector3d get_own_velocity(const RobotData& robot) const;
	virtual Vector3d get_others_velocity(const RobotData& robot) const;

	//Pretty much the same concept as in View::get_own_acceleration besides this methods are called by
	//View::get_robot_status.
	virtual RobotStatus get_own_status(const RobotData& robot) const;
	virtual RobotStatus get_others_status(const RobotData& robot) const;
	
	//Pretty much the same concept as in View::get_own_acceleration besides this methods are called by
	//View::get_robot_last_request_successful.
	virtual bool get_own_last_request_successful(const RobotData& robot) const;
	virtual bool get_others_last_request_successful(const RobotData& robot) const;

	//Called by non virtual View::is_point_in_obstacle
	virtual bool is_point_in_obstacle(const Obstacle& obstacle, const Vector3d& point) const;

	//Called by non virtual View::get_box_depth
	virtual double get_box_depth(const Box& box) const;
	//Called by non virtual View::get_box_width
	virtual double get_box_width(const Box& box) const;
	//Called by non virtual View::get_box_height
	virtual double get_box_height(const Box& box) const;

	//Called by non virtual View::get_sphere_radius
	virtual double get_sphere_radius(const Sphere& sphere) const;

	//some accesser for the use in sub classes
	const WorldInformation& world_information() const;
	const std::size_t get_id(boost::shared_ptr<Identifier> identifier) const;

private:
	//Following methods resolve a given *Ref to the corresponding data object of WorldInformation.
	//E.g. it resolves a given RobotRef to a RobotData object.
	const Obstacle& resolve_obstacle_ref(ObstacleRef obstacle) const;
	const RobotData& resolve_robot_ref(RobotRef robot) const;
	const WorldObject& resolve_marker_ref(MarkerRef marker) const;
	const Box& resolve_box_ref(BoxRef box) const;
	const Sphere& resolve_sphere_ref(SphereRef sphere) const;

	//Same as resolving methods above, but also checks if the given *Ref is valid.
	//To do so it compares the Identifier of the resolved data object with the given Identifier pointerwise.
	const Obstacle& resolve_obstacle_ref_safe(ObstacleRef obstacle) const;
	const RobotData& resolve_robot_ref_safe(RobotRef robot) const;
	const WorldObject& resolve_marker_ref_safe(MarkerRef marker) const;
	const Box& resolve_box_ref_safe(BoxRef box) const;
	const Sphere& resolve_sphere_ref_safe(SphereRef sphere) const;

	/**
	 * Helper methods to check if the given Identifier points to the same data object
	 * as the given Robots Identifier does. To do so it compares the internal id of the
	 * Identifiers. (Thats also why this method is defined as class method (needs friend access)).
	 */
	static bool is_own_identifier(const Robot& robot, boost::shared_ptr<RobotIdentifier> identifier);

	/**
	 * Given a set of four methods this method decides which to call. This is used by
	 * View::get_position for example to delegate the call the View::get_own_position,
	 * View::get_robot_position, View::get_obstacle_position, and View::get_marker_position.
	 * Which method is called is decided as in View::get_own_position described.
	 *
	 * The method uses resolve_*_ref_safe to resolve arguments for the given methods.
	 * A notable exception is the own_robot_function where resolve_robot_ref is used
	 * instead. This is because Identifier saved in Robot need not be same as saved
	 * in RobotData (pointerwise).
	 */
	template<typename T>
	T delegate_function(boost::function<T (const View*, const RobotData&)> own_robot_fun, boost::function<T (const View*, const RobotData&)> other_robot_fun,
						boost::function<T (const View*, const Obstacle&)> obstacle_fun, boost::function<T (const View*, const WorldObject&)> marker_fun,
						const Robot& caller, WorldObjectRef world_object) const;
	/**
	 * Same as above except that this method only decides between two methods as needed
	 * for get_robot_* methods.
	 * @see View::delgate_function
	 */
	template<typename T>
	T delegate_function(boost::function<T (const View*, const RobotData&)> own_robot_fun, boost::function<T (const View*, const RobotData&)> other_robot_fun,
	                    const Robot& caller, RobotRef robot) const;

private:
	boost::shared_ptr<WorldInformation> world_information_;

};

#endif /* VIEW_H_ */

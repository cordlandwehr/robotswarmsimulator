/**
 * \class	RobotData
 * \author	Martina Hüllmann
 * \brief	Contains the properties a robot can have, but
 * 			which are not necessarily visible to the robot
 * 			and belong to the WorldInformation.
 */

#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include "boost/tuple/tuple.hpp"
#include "../Utilities/vector3d.h"

using namespace std;

#include "world_object.h"

class Identifier;

//TODO(martinah) set possible stati of a robot
/**
 * \enum Status a robot can have.
 */
enum RobotStatus { STATUS };

//TODO(martinah) set possible types of a robot
/**
 * \enum Type a robot can have.
 */
enum RobotType { TYPE };

class RobotData : public WorldObject{
public:
	RobotData();
	~RobotData();

	/**
	 * Returns constant reference to the robot's id.
	 * \return Constant reference to the robot's id.
	 */
	const boost::shared_ptr<Identifier>& id() const;

	/**
	 * Returns constant reference to acceleration vector of the robot.
	 * \return Constant reference to acceleration vector of the robot.
	 */
	const Vector3d & acceleration() const;

	/**
	 * Sets acceleration of the robot.
	 * \param Pointer to acceleration vector.
	 */
	void set_acceleration(boost::shared_ptr<Vector3d> new_acceleration);

	/**
	 * Returns constant reference to triple of pointers to vectors
	 * containing the coordinate system axis.
	 * \return Triple of vectors containing the coordinate system axis.
	 */
	boost::tuple<boost::shared_ptr<Vector3d> > coordinate_system_axis() const;

	/**
	 * Returns type of the robot.
	 * \return type of the robot.
	 */
	const RobotType type() const;

	/**
	 * Returns constant reference to velocity vector of the robot.
	 * \return constant reference Velocity vector of the robot.
	 */
	const Vector3d & velocity() const;

	/**
	 * Sets velocity of the robot.
	 * \param Pointer to new velocity vector.
	 */
	void set_velocity(boost::shared_ptr<Vector3d> new_velocity);

	/**
	 * Returns status of the robot.
	 * \return Status of the robot.
	 */
	RobotStatus status() const;

	/**
	 * Sets status of the robot.
	 * \param new status
	 */
	void set_status(RobotStatus new_status);

private:
	boost::shared_ptr<Identifier> id_;
	boost::shared_ptr<Vector3d> acceleration_;
	/**
	 * \var Triple with the three coordinate axis of the robot.
	 */
	boost::tuple<boost::shared_ptr<Vector3d> > coordinate_system_axis_;
	RobotType type_;
	boost::shared_ptr<Vector3d> velocity_;
	RobotStatus status_;
};

#endif /* ROBOT_DATA_H_ */

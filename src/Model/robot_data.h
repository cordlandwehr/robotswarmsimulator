/**
 * \class	RobotData
 * \author	Martina Hüllmann
 * \brief	Contains the properties a robot can have, but
 * 			which are not necessarily visible to the robot
 * 			and belong to the WorldInformation.
 */

#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <boost/tuple/tuple.hpp>
#include "../Utilities/vector3d.h"

using namespace std;

#include "world_object.h"

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

class Robot;

class RobotData : public WorldObject{
public:
	RobotData(const Robot& robot);
	~RobotData();

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
	boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> >
			coordinate_system_axis() const;

	/**
	 * Sets the coordinate system to the given triple of vectors.
	 * \return Triple of vectors for new axes.
	 */
	void set_coordinate_system_axis(boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
			boost::shared_ptr<Vector3d> > new_axes);

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

	/**
	 * Returns reference to according robot-object.
	 * \return Reference to according robot-object.
	 */
	const Robot& get_robot() const;


private:
	/**
	 * Reference to according robot.
	 */
	const Robot& robot_;
	boost::shared_ptr<Vector3d> acceleration_;
	/**
	 * \var Triple with the three coordinate axes of the robot.
	 */
	boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > coordinate_system_axis_;
	RobotType type_;
	boost::shared_ptr<Vector3d> velocity_;
	RobotStatus status_;
};

#endif /* ROBOT_DATA_H_ */

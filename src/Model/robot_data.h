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

using namespace std;

#include "world_object.h"

class Identifier;

//TODO set possible stati of a robot
/**
 * \enum Status a robot can have.
 */
enum Status { STATUS };

//TODO set possible types of a robot
/**
 * \enum Type a robot can have.
 */
enum Type { TYPE };

class RobotData : public WorldObject{
public:
	RobotData();
	virtual ~RobotData();

	/**
	 * Returns constant reference to the robot's id.
	 * \return Constant reference to the robot's id.
	 */
	const boost::shared_ptr<Identifier>& id() const;

	/**
	 * Returns acceleration vector of the robot.
	 * \return Acceleration vector of the robot.
	 */
	std::vector<double> acceleration() const;

	/**
	 * Set acceleration of the robot
	 * \param acceleration vector
	 */
	void set_acceleration(std::vector<double> new_acceleration);

	/**
	 * Returns triple of vectors containing the coordinate system axis.
	 * \return Triple of vectors containing the coordinate system axis.
	 */
	const boost::tuple<std::vector<double> > coordinate_system_axis() const;

	/**
	 * Returns type of the robot.
	 * \return type of the robot.
	 */
	const Type type() const;

	/**
	 * Returns velocity vector of the robot.
	 * \return Velocity vector of the robot.
	 */
	std::vector<double> velocity() const;

	/**
	 * Sets velocity of the robot.
	 * \param new velocity vector.
	 */
	void set_velocity(std::vector<double> new_velocity);

	/**
	 * Returns status of the robot.
	 * \return Status of the robot.
	 */
	Status status() const;

	/**
	 * Sets status of the robot.
	 * \param new status
	 */
	void set_status(Status new_status);

private:
	const boost::shared_ptr<Identifier> id_;
	std::vector<double> acceleration_;
	/**
	 * \var Triple with the three coordinate axis of the robot.
	 */
	const boost::tuple<vector<double> > coordinate_system_axis_;
	Type type_;
	std::vector<double> velocity_;
	Status status_;
};

#endif /* ROBOT_DATA_H_ */

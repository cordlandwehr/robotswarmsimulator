/*
 * robot_control.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef ROBOT_CONTROL_H_
#define ROBOT_CONTROL_H_

#include <set>
#include <boost/shared_ptr.hpp>
#include "simulation_listener.h"

class Request;
class Robot;

/**
 * \class RobotControl
 *
 * Controls the Robots. Responsible for assigning the View to each Robot and
 * calling the compute method when requested.
 *
 * RobotControl most likely holds a buffer of Views of length history_length. To do this, the
 * RobotControl provides an update method.
 *
 */
class RobotControl {
public:
	/**
	 * Constructs a new RobotControl.
	 * @param The view_factory given determines which
	 * view model should be used for the robots
	 * @param The length of the history
	 * @param intial WorldInformation
	 * @see ModelParameters::HISTORY_LENGTH
	 */
	RobotControl();
	virtual ~RobotControl();

	virtual void update(const WorldInformation& world_information) = 0;

	/**
	 * Equivalent to robot.compute().
	 * @param robot
	 * @param Set of Requests
	 * @see Robot::compute()
	 */
	std::set<boost::shared_ptr<Request> > compute_new_request(Robot& robot);
	/**
	 * Computes and assigns the newest View to the given robot.
	 * @param robot
	 */
	virtual void compute_view(Robot& robot) = 0;
};

#endif /* ROBOT_CONTROL_H_ */

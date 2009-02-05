/*
 * robot_control.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef ROBOT_CONTROL_H_
#define ROBOT_CONTROL_H_

#include <set>
#include <cstddef>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include "simulation_listener.h"

class AbstractViewFactory;
class Request;
class Robot;
class View;
/**
 * \class RobotControl
 *
 * Controls the Robots. Responsible for assigning the View to each Robot and
 * calling the compute method when requested.
 *
 * RobotControl holds a buffer of Views of length history_length. To do this, the
 * RobotControl implements the SimulationListener interface. Whenever update is called
 * together with an HandleRequestsEvent RobotControl creates a new View
 * for the new WorldInfortion.
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
	RobotControl(boost::shared_ptr<AbstractViewFactory> view_factory, std::size_t history_length, const WorldInformation& initial_world_information);
	~RobotControl();

	void update(const WorldInformation& world_information);

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
	void compute_view(Robot& robot);

	void set_view_facotry(const boost::shared_ptr<AbstractViewFactory>& view_factory);
private:
	boost::shared_ptr<AbstractViewFactory> view_factory_;
	boost::circular_buffer<boost::shared_ptr<View> > view_buffer_;
};

#endif /* ROBOT_CONTROL_H_ */

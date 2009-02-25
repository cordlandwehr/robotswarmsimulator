/*
 * uniform_robot_control.h
 *
 *  Created on: 25.02.2009
 *      Author: Daniel
 */

#ifndef UNIFORM_ROBOT_CONTROL_H_
#define UNIFORM_ROBOT_CONTROL_H_

#include "robot_control.h"

#include <cstddef>
#include <boost/circular_buffer.hpp>

class AbstractViewFactory;
class View;

/**
 * \class UniformRobotControl
 *
 * \brief RobotControl which assigns the same view type to all Robots.
 *
 */
class UniformRobotControl : public RobotControl {
public:
	/**
	 * Constructs a new UniformRobotControl.
	 * @param The view_factory given determines which
	 * view model should be used for the robots
	 * @param The length of the history
	 * @param intial WorldInformation
	 * @see ModelParameters::HISTORY_LENGTH
	 */
	UniformRobotControl(boost::shared_ptr<AbstractViewFactory> view_factory, std::size_t history_length, const WorldInformation& initial_world_information);
	virtual ~UniformRobotControl();

	virtual void update(const WorldInformation& world_information);
	virtual void compute_view(Robot& robot);

	void set_view_factory(const boost::shared_ptr<AbstractViewFactory>& view_factory);
private:
	boost::shared_ptr<AbstractViewFactory> view_factory_;
	boost::circular_buffer<boost::shared_ptr<View> > view_buffer_;
};

#endif /* UNIFORM_ROBOT_CONTROL_H_ */

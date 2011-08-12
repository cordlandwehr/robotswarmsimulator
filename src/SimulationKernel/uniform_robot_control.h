/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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
	UniformRobotControl(boost::shared_ptr<AbstractViewFactory> view_factory, std::size_t history_length, const boost::shared_ptr<WorldInformation>& initial_world_information);
	virtual ~UniformRobotControl();

	virtual void update(const boost::shared_ptr<WorldInformation>& world_information);
	virtual void compute_view(Robot& robot);

	void set_view_factory(const boost::shared_ptr<AbstractViewFactory>& view_factory);
private:
	boost::shared_ptr<AbstractViewFactory> view_factory_;
	boost::circular_buffer<boost::shared_ptr<View> > view_buffer_;
	boost::shared_ptr<WorldInformation> last_world_information_;
};

#endif /* UNIFORM_ROBOT_CONTROL_H_ */

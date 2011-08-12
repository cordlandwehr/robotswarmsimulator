/*
	This file is part of RobotSwarmSimulator.

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
 * robot_type_robot_control.h
 *
 *  Created on: 25.02.2009
 *      Author: Daniel
 */

#ifndef ROBOT_TYPE_ROBOT_CONTROL_H_
#define ROBOT_TYPE_ROBOT_CONTROL_H_

#include "robot_control.h"

#include "../Model/robot_data.h"

#include <boost/array.hpp>
#include <cstddef>
#include <boost/circular_buffer.hpp>

class AbstractViewFactory;
class View;

class RobotTypeRobotControl: public RobotControl {
public:
	RobotTypeRobotControl(const boost::array<boost::shared_ptr<AbstractViewFactory>,kRobotTypeCount>& view_factories, std::size_t history_length, const boost::shared_ptr<WorldInformation>& initial_world_information);
	virtual ~RobotTypeRobotControl();

	virtual void update(const boost::shared_ptr<WorldInformation>& world_information);
	virtual void compute_view(Robot& robot);

private:
	boost::shared_ptr<boost::array<boost::shared_ptr<View>,kRobotTypeCount> > create_views(const boost::shared_ptr<WorldInformation>& world_information);
	
private:
	boost::array<boost::shared_ptr<AbstractViewFactory>,kRobotTypeCount> view_factories_;
	boost::circular_buffer<boost::shared_ptr<boost::array<boost::shared_ptr<View>,kRobotTypeCount> > > view_buffer_;
	boost::shared_ptr<WorldInformation> last_world_information_;
};

#endif /* ROBOT_TYPE_ROBOT_CONTROL_H_ */

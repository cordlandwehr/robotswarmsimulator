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

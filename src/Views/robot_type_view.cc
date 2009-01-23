/*
 * robot_type_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "robot_type_view.h"

RobotTypeView::RobotTypeView() {


}

RobotTypeView::~RobotTypeView() {

}

RobotType RobotTypeView::get_others_type(const RobotData& robot) const {
	return robot.type();
}

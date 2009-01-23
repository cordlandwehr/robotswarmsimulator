/*
 * robot_status_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "robot_status_view.h"

RobotStatusView::RobotStatusView() {


}

RobotStatusView::~RobotStatusView() {

}

RobotStatus RobotStatusView::get_others_status(const RobotData& robot) const {
	return robot.status();
}

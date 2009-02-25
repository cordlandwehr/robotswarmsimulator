/*
 * robot_control.cc
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#include "robot_control.h"

#include "../Model/robot.h"

RobotControl::RobotControl() {

}

RobotControl::~RobotControl() {

}


std::set<boost::shared_ptr<Request> > RobotControl::compute_new_request(Robot& robot) {
	return robot.compute();
}

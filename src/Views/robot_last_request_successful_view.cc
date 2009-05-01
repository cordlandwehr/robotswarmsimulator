/*
 *  robot_last_request_successful_view.cc
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 01.05.09.
 */

#include "robot_last_request_successful_view.h"

RobotLastRequestSuccessfulView::RobotLastRequestSuccessfulView() {
}

RobotLastRequestSuccessfulView::~RobotLastRequestSuccessfulView() {
}

bool RobotLastRequestSuccessfulView::get_others_last_request_successful(const RobotData& robot) const {
	return robot.last_request_successful();
}

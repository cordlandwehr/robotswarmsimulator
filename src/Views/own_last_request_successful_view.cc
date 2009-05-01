/*
 *  own_last_request_successful_view.cc
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 01.05.09.
 */

#include "own_last_request_successful_view.h"

OwnLastRequestSuccessfulView::OwnLastRequestSuccessfulView() {
}

OwnLastRequestSuccessfulView::~OwnLastRequestSuccessfulView() {
}

bool OwnLastRequestSuccessfulView::get_own_last_request_successful(const RobotData& robot) const {
	return robot.last_request_successful();
}

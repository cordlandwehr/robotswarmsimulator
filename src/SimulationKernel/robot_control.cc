/*
 * robot_control.cc
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#include "robot_control.h"

RobotControl::RobotControl(boost::shared_ptr<ViewFactory> view_factory) : view_factory_(view_factory) {

}

RobotControl::~RobotControl() {

}

void RobotControl::update(const WorldInformation& world_information, boost::shared_ptr<Event> last_event) {
	//TODO:
}

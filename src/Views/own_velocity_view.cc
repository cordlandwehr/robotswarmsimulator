/*
 * own_velocity_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_velocity_view.h"

OwnVelocityView::OwnVelocityView() {

}

OwnVelocityView::~OwnVelocityView() {

}

Vector3d OwnVelocityView::get_own_velocity(const RobotData& robot) const {
	return robot.velocity();
}

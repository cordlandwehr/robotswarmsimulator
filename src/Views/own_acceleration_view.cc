/*
 * own_acceleration_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_acceleration_view.h"

OwnAccelerationView::OwnAccelerationView() {

}

OwnAccelerationView::~OwnAccelerationView() {

}

Vector3d OwnAccelerationView::get_own_acceleration(const RobotData& robot) const {
	return robot.acceleration();
}

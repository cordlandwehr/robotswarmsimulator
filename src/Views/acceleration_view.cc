/*
 * acceleration_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "acceleration_view.h"

AccelerationView::AccelerationView() {

}

AccelerationView::~AccelerationView() {
}

Vector3d AccelerationView::get_others_acceleration(const RobotData& robot) const {
	return robot.acceleration();
}

/*
 * velocity_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "velocity_view.h"

VelocityView::VelocityView() {


}

VelocityView::~VelocityView() {

}

Vector3d VelocityView::get_others_velocity(const RobotData& robot) const {
	return robot.velocity();
}

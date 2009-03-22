/*
 * own_origin_view.cc
 *
 *  Created on: 22.03.2009
 *      Author: Daniel Wonisch
 */

#include <Views/own_origin_view.h>

OwnOriginView::OwnOriginView() {

}

OwnOriginView::~OwnOriginView() {

}

Vector3d OwnOriginView::get_own_coordinate_system_origin(const RobotData & robot) const {
	return robot.position();
}
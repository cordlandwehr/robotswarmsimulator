/*
 * origin_view.cc
 *
 *  Created on: 22.03.2009
 *      Author: Daniel Wonisch
 */

#include <Views/origin_view.h>

OriginView::OriginView() {

}

OriginView::~OriginView() {

}

Vector3d OriginView::get_others_coordinate_system_origin(const RobotData & robot) const {
	return robot.position();
}
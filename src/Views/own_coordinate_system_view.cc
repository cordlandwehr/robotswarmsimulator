/*
 * own_coordinate_system_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_coordinate_system_view.h"

OwnCoordinateSystemView::OwnCoordinateSystemView() {

}

OwnCoordinateSystemView::~OwnCoordinateSystemView() {

}

boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > OwnCoordinateSystemView::get_own_coordinate_system_axis(const RobotData& robot) const {
	return robot.coordinate_system_axis();
}

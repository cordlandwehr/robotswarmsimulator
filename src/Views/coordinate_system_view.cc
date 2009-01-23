/*
 * coordinate_system_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "coordinate_system_view.h"

CoordinateSystemView::CoordinateSystemView() {


}

CoordinateSystemView::~CoordinateSystemView() {

}

boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > CoordinateSystemView::get_others_coordinate_system_axis(const RobotData& robot) const {
	return robot.coordinate_system_axis();
}


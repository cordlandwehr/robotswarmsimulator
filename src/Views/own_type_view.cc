/*
 * own_type_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_type_view.h"

OwnTypeView::OwnTypeView() {

}

OwnTypeView::~OwnTypeView() {

}

RobotType OwnTypeView::get_own_type(const RobotData& robot) const {
	return robot.type();
}

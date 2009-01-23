/*
 * own_position_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_position_view.h"

OwnPositionView::OwnPositionView() {


}

OwnPositionView::~OwnPositionView() {

}

Vector3d OwnPositionView::get_own_position(const RobotData& robot) const {
	return robot.position();
}

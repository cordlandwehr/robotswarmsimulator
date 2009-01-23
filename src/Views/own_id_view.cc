/*
 * own_id_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_id_view.h"

OwnIdView::OwnIdView() {


}

OwnIdView::~OwnIdView() {

}

std::size_t OwnIdView::get_own_id(const RobotData& robot) const {
	return get_id(robot.id());
}

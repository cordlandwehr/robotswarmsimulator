/*
 * own_status_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_status_view.h"

OwnStatusView::OwnStatusView() {


}

OwnStatusView::~OwnStatusView() {

}

RobotStatus OwnStatusView::get_own_status(const RobotData& robot) const {
	return robot.status();
}

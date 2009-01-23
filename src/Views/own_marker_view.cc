/*
 * own_marker_view.cc
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#include "own_marker_view.h"

OwnMarkerView::OwnMarkerView() {


}

OwnMarkerView::~OwnMarkerView() {

}

MarkerInformation OwnMarkerView::get_own_marker_information(const RobotData& robot) const {
	return robot.marker_information();
}

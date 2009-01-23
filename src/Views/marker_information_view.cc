/*
 * marker_information_view.cpp
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "marker_information_view.h"
#include "../Model/obstacle.h"

MarkerInformationView::MarkerInformationView() {


}

MarkerInformationView::~MarkerInformationView() {

}

MarkerInformation MarkerInformationView::get_robots_marker_information(const RobotData& robot) const {
	return robot.marker_information();
}
MarkerInformation MarkerInformationView::get_obstacles_marker_information(const Obstacle& obstacle) const {
	return obstacle.marker_information();
}
MarkerInformation MarkerInformationView::get_markers_marker_information(const WorldObject& marker) const {
	return marker.marker_information();
}

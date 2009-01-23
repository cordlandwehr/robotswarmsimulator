/*
 * marker_information_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef MARKER_INFORMATION_VIEW_H_
#define MARKER_INFORMATION_VIEW_H_

#include "view.h"

class MarkerInformationView: public virtual View {
public:
	MarkerInformationView();
	virtual ~MarkerInformationView();

protected:
	virtual MarkerInformation get_own_marker_information(const RobotData& robot) const;
	virtual MarkerInformation get_robots_marker_information(const RobotData& robot) const;
	virtual MarkerInformation get_obstacles_marker_information(const Obstacle& obstacle) const;
	virtual MarkerInformation get_markers_marker_information(const WorldObject& marker) const;
};

#endif /* MARKER_INFORMATION_VIEW_H_ */

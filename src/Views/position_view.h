/*
 * position_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef POSITION_VIEW_H_
#define POSITION_VIEW_H_

#include "own_position_view.h"

class PositionView : public virtual OwnPositionView {
public:
	PositionView();
	virtual ~PositionView();

protected:
	virtual Vector3d get_robot_position(const RobotData& robot) const;
	virtual Vector3d get_obstacle_position(const Obstacle& obstacle) const;
	virtual Vector3d get_marker_position(const WorldObject& marker) const;
};

#endif /* POSITION_VIEW_H_ */

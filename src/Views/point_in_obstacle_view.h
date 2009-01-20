/*
 * point_in_obstacle_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef POINT_IN_OBSTACLE_VIEW_H_
#define POINT_IN_OBSTACLE_VIEW_H_

#include "view.h"

class PointInObstacleView: public virtual View {
public:
	PointInObstacleView();
	virtual ~PointInObstacleView();

protected:
	virtual bool is_point_in_obstacle(const Obstacle& obstacle, const Vector3d& point) const;
};

#endif /* POINT_IN_OBSTACLE_VIEW_H_ */

/*
 * point_in_obstacle_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "point_in_obstacle_view.h"
#include "../Model/obstacle.h"

PointInObstacleView::PointInObstacleView() {

}

PointInObstacleView::~PointInObstacleView() {

}

bool PointInObstacleView::is_point_in_obstacle(const Obstacle& obstacle, const Vector3d& point) const {
	return obstacle.contains_point(boost::shared_ptr<Vector3d>(new Vector3d(point)));
}

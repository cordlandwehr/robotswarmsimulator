/*
 * point_in_obstacle_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "point_in_obstacle_view.h"
#include "../Model/obstacle.h"

PointInObstacleView::PointInObstacleView() {
	// TODO Auto-generated constructor stub

}

PointInObstacleView::~PointInObstacleView() {
	// TODO Auto-generated destructor stub
}

bool PointInObstacleView::is_point_in_obstacle(const Obstacle& obstacle, const Vector3d& point) const {
	//TODO (dwonisch): I dont like this.
	return obstacle.contains_point(boost::shared_ptr<Vector3d>(new Vector3d(point)));
}

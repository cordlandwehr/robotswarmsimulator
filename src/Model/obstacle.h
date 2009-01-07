/**
 * \class	WorldObject
 * \author	Martina Hüllmann
 * \brief	Denotes an obstacle in the world.
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "world_object.h"

class Obstacle : public WorldObject {
public:
	Obstacle();
	virtual ~Obstacle();

	/**
	 * Checks whether the given point is contained in the obstacle.
	 * \param point to check whether it's contained in the obstacle.
	 */
	bool contains_point(std::vector<double> point);
};

#endif /* OBSTACLE_H_ */

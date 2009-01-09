/**
 * \class	WorldObject
 * \author	Martina Hüllmann
 * \brief	Denotes an obstacle in the world.
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include <boost/smart_ptr.hpp>

#include "world_object.h"
#include "../Utilities/vector3d.h"

class Obstacle : public WorldObject {
public:
	Obstacle();
	virtual ~Obstacle();

	/**
	 * Checks whether the given point is contained in the obstacle.
	 * \param Pointer to vector of point to check whether it's
	 *        contained in the obstacle.
	 */
	bool contains_point(boost::shared_ptr<Vector3d> point);
};

#endif /* OBSTACLE_H_ */

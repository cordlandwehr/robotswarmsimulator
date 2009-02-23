/**
 * \class	WorldObject
 * \author	Martina Hüllmann
 * \brief	Denotes an obstacle in the world.
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include <boost/shared_ptr.hpp>

#include "world_object.h"
#include "../Utilities/vector_arithmetics.h"


class Obstacle : public WorldObject {
public:
	Obstacle(boost::shared_ptr<Identifier> id,
			 boost::shared_ptr<Vector3d> position);
	Obstacle(boost::shared_ptr<Identifier> id,
             boost::shared_ptr<Vector3d> position,
             boost::shared_ptr<MarkerInformation> marker_information);
	virtual ~Obstacle();

	/**
	 * Checks whether the given point is contained in the obstacle.
	 * \param Pointer to vector of point to check whether it's
	 *        contained in the obstacle.
	 */
	virtual bool contains_point(boost::shared_ptr<Vector3d> point) const = 0;
	virtual boost::shared_ptr<WorldObject> clone() const = 0;
	virtual double max_dimension() const = 0;
};

#endif /* OBSTACLE_H_ */

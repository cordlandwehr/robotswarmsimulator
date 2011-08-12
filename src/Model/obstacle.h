/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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

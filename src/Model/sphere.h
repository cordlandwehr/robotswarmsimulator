/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
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
 * \class	Sphere
 * \author	Martina Hüllmann
 * \brief	Denotes a sphere-obstacle
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "obstacle.h"


class Sphere : public Obstacle {
public:
	Sphere(boost::shared_ptr<Identifier> id,
	       boost::shared_ptr<Vector3d> position,
	       double radius);
	Sphere(boost::shared_ptr<Identifier> id,
           boost::shared_ptr<Vector3d> position,
	       boost::shared_ptr<MarkerInformation> marker_information,
	       double radius);
	~Sphere();

	/**
	 * Returns the radius of the sphere.
	 * \return Radius of the sphere.
	 */
	double radius() const;

	/**
	 * Sets the radius of the sphere to the given value.
	 * \param New radius of the sphere.
	 */
	void set_radius(double new_radius);

	/**
	 * Checks whether the given point is contained in the obstacle.
	 * \param Pointer to vector of point to check whether it's
	 *        contained in the obstacle.
	 */
	bool contains_point(boost::shared_ptr<Vector3d> point) const;

	virtual boost::shared_ptr<WorldObject> clone() const;

	double max_dimension() const{ return radius_; }
private:
	double radius_;
};

#endif /* SPHERE_H_ */

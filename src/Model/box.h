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
 * \class	Box
 * \author	Martina Hüllmann
 * \brief	Denotes an box-obstacle
 */

#ifndef BOX_H_
#define BOX_H_

#include "obstacle.h"
#include "../Utilities/vector_arithmetics.h"


class Box : public Obstacle {
public:
	Box(boost::shared_ptr<Identifier> id,
		boost::shared_ptr<Vector3d> position,
		double depth,
		double width,
		double height);
	Box(boost::shared_ptr<Identifier> id,
	    boost::shared_ptr<Vector3d> position,
        boost::shared_ptr<MarkerInformation> marker_information,
        double depth,
       	double width,
       	double height);
	~Box();

	/**
	 * Returns the length of the box.
	 * \return Length of the box.
	 */
	double height() const;

	/**
	 * Sets the height of the box to the given value.
	 * \param New height of the box.
	 */
	void set_height(double new_height);

	/**
	 * Returns the depth of the box.
	 * \return Depth of the box.
	 */
	double depth() const;

	/**
	 * Sets the depth of the box to the given value.
	 * \param New depth of the box.
	 */
	void set_depth(double new_depth);

	/**
	 * Returns the width of the box.
	 * \return Width of the box.
	 */
	double width() const;

	/**
	 * Sets the width of the box to the given value.
	 * \param New width of the box.
	 */
	void set_width(double new_width);

	/**
	 * Checks whether the given point is contained in the obstacle.
	 * \param Pointer to vector of point to check whether it's contained
	 * 	      in the obstacle.
	 */
	bool contains_point(boost::shared_ptr<Vector3d> point) const;

	virtual boost::shared_ptr<WorldObject> clone() const;

	double max_dimension() const;

private:
	/**
	 * \var Depth (in z-direction) of the box.
	 */
	double depth_;
	/**
	 * \var Width (in x-direction) of the box.
	 */
	double width_;
	/**
	 * \var Height in (y-direction) of the box.
	 */
	double height_;
};

#endif /* BOX_H_ */

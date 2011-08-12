/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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

/*
 * points_sepplane.h
 *
 *  Created on: 15.07.2009
 *      Author: peter
 *
 */

#ifndef POINTS_SEPPLANE_H_
#define POINTS_SEPPLANE_H_


#include "../Utilities/vector_arithmetics.h"


/**
 * \brief Computes a separation plane with maximum distance to v that separates v from all points in w.
 *
 * Vector w has to contain at least one point (otherwise: behaviour unspecified).
 *
 * Any duplicates of v in w will be ignored. However, if all points in w are equal to v, a null pointer is returned (v
 * lies in the convex hull of the w's).
 *
 * The plane is returned in the following way:
 *     - plane's normal vector: -returned_vector
 *     - point on plane: v+returned_vector
 * If no such plane exists, the null-vector is returned.
 *
 * As a second parameter, one has to specify an epsilon value that is used to filter points from w that are nearly equal
 * to v (needed because of numeric inaccuracies).
 */
const Vector3d separate_point_from_points(const Vector3d& v, const std::vector<Vector3d>& w, double epsilon);


#endif /* POINTS_SEPPLANE_H_ */

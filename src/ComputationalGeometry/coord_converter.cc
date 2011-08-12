/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 * coord_converter.cc
 *
 *  Created on: 15.01.2009
 *      Author: Alexander Klaas
 */

#include <boost/shared_ptr.hpp>
#include "coord_converter.h"

boost::shared_ptr<Vector3d> CoordConverter::global_to_local(const Vector3d& absolute_coord, const Vector3d& origin,
                                         const boost::tuple<boost::shared_ptr<Vector3d>,
                                                            boost::shared_ptr<Vector3d>,
                                                            boost::shared_ptr<Vector3d> >& local_coord_system) {
	using boost::numeric::ublas::prod;
	
	// translate point in absolute space according to origin
	Vector3d global_translated = absolute_coord - origin;

	// transform to given base and return
	Matrix3d base(3,3);
	for (int i=0; i<3; i++) {
		base(i,0) = (*boost::get<0>(local_coord_system))(i);
		base(i,1) = (*boost::get<1>(local_coord_system))(i);
		base(i,2) = (*boost::get<2>(local_coord_system))(i);
	}
	Matrix3d base_inverted = invert_matrix3d(base);
	boost::shared_ptr<Vector3d> converted_vector(new Vector3d(prod(base_inverted, global_translated)));
	return converted_vector;
}

boost::shared_ptr<Vector3d> CoordConverter::local_to_global(const Vector3d& local_coord,
                                                            const Vector3d& local_origin,
                                         const boost::tuple<boost::shared_ptr<const Vector3d>,
                                                            boost::shared_ptr<const Vector3d>,
                                                            boost::shared_ptr<const Vector3d> >& local_coord_system) {
	boost::shared_ptr<Vector3d> result(new Vector3d());

	// compute the result vector by multiplying the coefficents (in local_coord) with the bases
	result->insert_element(kXCoord, local_coord(kXCoord) * (*boost::get<0>(local_coord_system))(kXCoord) +
	                               local_coord(kYCoord) * (*boost::get<1>(local_coord_system))(kXCoord) +
	                               local_coord(kZCoord) * (*boost::get<2>(local_coord_system))(kXCoord));

	result->insert_element(kYCoord, local_coord(kXCoord) * (*boost::get<0>(local_coord_system))(kYCoord) +
		                           local_coord(kYCoord) * (*boost::get<1>(local_coord_system))(kYCoord) +
		                           local_coord(kZCoord) * (*boost::get<2>(local_coord_system))(kYCoord));

	result->insert_element(kZCoord, local_coord(kXCoord) * (*boost::get<0>(local_coord_system))(kZCoord) +
		                           local_coord(kYCoord) * (*boost::get<1>(local_coord_system))(kZCoord) +
		                           local_coord(kZCoord) * (*boost::get<2>(local_coord_system))(kZCoord));
	// add the origin
	*result = *result + local_origin;
	return result;
}

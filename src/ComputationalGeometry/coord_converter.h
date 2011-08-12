/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
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
 * coord_converter.h
 *
 *  Created on: 15.01.2009
 *      Author: Alexander Klaas
 */

#ifndef COORD_CONVERTER_H_
#define COORD_CONVERTER_H_

#include "../Utilities/vector_arithmetics.h"
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>


/**
 * \brief Namespace providing utility methods to convert global to local coordinates and vice versa.
 */
namespace CoordConverter {
	/**
	 * calculates local coordinates from global ones
	 * @param absolute_coord: the global cordinate
	 * @param origin: the origin in global coordinates (typically a robot position)
	 * @param local_coord_system: three base vectors
	 * \return the local coordinate
	 */
	boost::shared_ptr<Vector3d> global_to_local(const Vector3d& absolute_coord, const Vector3d& origin,
	                                            const boost::tuple<boost::shared_ptr<Vector3d>,
	                                            boost::shared_ptr<Vector3d>,
	                                            boost::shared_ptr<Vector3d> >& local_coord_system);

	/**
	 * calculates global coordinates from local ones. The origin of global systems is assumed to be (0,0,0).
	 * The global coordinate system is assumed to use unit vectors as base.
	 * @param local_coord: the local coordinate
	 * @param local_coord_system: the three base vectors of the local coordinate system
	 * \return the global coordinate
	 *
	 */
	boost::shared_ptr<Vector3d> local_to_global(const Vector3d& local_coord,
	                                            const Vector3d& local_origin,
	                                            const boost::tuple<boost::shared_ptr<const Vector3d>,
	                                            boost::shared_ptr<const Vector3d>,
	                                            boost::shared_ptr<const Vector3d> >& local_coord_system);
};

#endif /* COORD_CONVERTER_H_ */

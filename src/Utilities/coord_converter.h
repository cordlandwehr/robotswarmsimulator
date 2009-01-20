/*
 * coord_converter.h
 *
 *  Created on: 15.01.2009
 *      Author: Alexander Klaas
 */

#ifndef COORD_CONVERTER_H_
#define COORD_CONVERTER_H_

#include "../Utilities/vector3d.h"
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
using namespace std;


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
	 * calculates global coordinates from local ones. The origin of both systems is assumed to be (0,0,0).
	 * The global coordinate system is assumed to use unit vectors as base.
	 * @param local_coord: the local coordinate
	 * @param local_coord_system: the three base vectors of the local coordinate system
	 * \return the global coordinate
	 *
	 */
	boost::shared_ptr<Vector3d> local_to_global(const Vector3d& local_coord,
	                                            const boost::tuple<boost::shared_ptr<const Vector3d>,
	                                            boost::shared_ptr<const Vector3d>,
	                                            boost::shared_ptr<const Vector3d> >& local_coord_system);
};

#endif /* COORD_CONVERTER_H_ */

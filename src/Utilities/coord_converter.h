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
 * \class CoordConverter
 * \brief Utility class to convert global to local coordinates and vice versa
 *
 * This class provides functions for converting coordinates between respective coordinate systems
 *
 * Abstract class, because it should not be instantiated, utility methods are static.
 *
 *
 */
class CoordConverter {
public:
	CoordConverter();
	virtual ~CoordConverter() = 0;

	/**
		 * calculates local coordinates from global ones
		 * @param absolute_coord: the global cordinate
		 * @param origin: the origin in global coordinates (typically a robot position)
		 * @param local_coord_system: three base vectors
		 * \return the local coordinate
		 */
	static Vector3d global_to_local(const Vector3d &absolute_coord, const Vector3d  &origin, boost::tuple<boost::shared_ptr<Vector3d> ,boost::shared_ptr<Vector3d> ,boost::shared_ptr<Vector3d> > local_coord_system);
	static Vector3d local_to_global(const Vector3d &local_coord, const Vector3d  &origin, boost::tuple<boost::shared_ptr<Vector3d> ,boost::shared_ptr<Vector3d> ,boost::shared_ptr<Vector3d> > local_coord_system);
};

#endif /* COORD_CONVERTER_H_ */
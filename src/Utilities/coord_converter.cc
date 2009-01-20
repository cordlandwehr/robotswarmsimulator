/*
 * coord_converter.cc
 *
 *  Created on: 15.01.2009
 *      Author: Alexander Klaas
 */

#include "coord_converter.h"

Vector3d CoordConverter::global_to_local(const Vector3d& absolute_coord, const Vector3d& origin,
                                         const boost::tuple<boost::shared_ptr<Vector3d>,
                                                            boost::shared_ptr<Vector3d>,
                                                            boost::shared_ptr<Vector3d> >& local_coord_system)
{
	Vector3d global_translated;
//first, translate point in absolute space according to origin
	global_translated(0)=absolute_coord(0)-origin(0);
	global_translated(1)=absolute_coord(1)-origin(1);
	global_translated(2)=absolute_coord(2)-origin(2);

	//cout << "global translated: " << global_translated(0) << global_translated(1) << global_translated(2);

	//TODO: then, transform to given base and return


	return global_translated;
}

Vector3d CoordConverter::local_to_global(const Vector3d& local_coord,
                                         const boost::tuple<boost::shared_ptr<const Vector3d>,
                                                            boost::shared_ptr<const Vector3d>,
                                                            boost::shared_ptr<const Vector3d> >& local_coord_system) {
	Vector3d result;

	// compute the result vector by multiplying the coefficents (in local_coord) with the bases
	result.insert_element(kXCoord, local_coord(kXCoord) * (*boost::get<0>(local_coord_system))(kXCoord) +
	                               local_coord(kYCoord) * (*boost::get<1>(local_coord_system))(kXCoord) +
	                               local_coord(kZCoord) * (*boost::get<2>(local_coord_system))(kXCoord));

	result.insert_element(kYCoord, local_coord(kXCoord) * (*boost::get<0>(local_coord_system))(kYCoord) +
		                           local_coord(kYCoord) * (*boost::get<1>(local_coord_system))(kYCoord) +
		                           local_coord(kZCoord) * (*boost::get<2>(local_coord_system))(kYCoord));

	result.insert_element(kZCoord, local_coord(kXCoord) * (*boost::get<0>(local_coord_system))(kZCoord) +
		                           local_coord(kYCoord) * (*boost::get<1>(local_coord_system))(kZCoord) +
		                           local_coord(kZCoord) * (*boost::get<2>(local_coord_system))(kZCoord));

	return result;
}





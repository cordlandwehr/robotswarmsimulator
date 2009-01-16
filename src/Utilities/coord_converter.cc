/*
 * coord_converter.cc
 *
 *  Created on: 15.01.2009
 *      Author: Alexander Klaas
 */

#include "coord_converter.h"

CoordConverter::~CoordConverter() {}

Vector3d CoordConverter::global_to_local(const Vector3d &absolute_coord, const Vector3d  &origin, boost::tuple<boost::shared_ptr<Vector3d> ,boost::shared_ptr<Vector3d> ,boost::shared_ptr<Vector3d> > local_coord_system)
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







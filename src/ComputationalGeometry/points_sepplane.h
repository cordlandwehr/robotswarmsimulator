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
 * If such a plane does not exist (v is contained in the convex hull of the w's), a null pointer is returned. Otherwise,
 * the corresponding plane is returned. Vector w has to contain at least one point (otherwise: behaviour unspecified).
 *
 * Any duplicates of v in w will be ignored. However, if all points in w are equal to v, a null pointer is returned (v
 * lies in the convex hull of the w's).
 *
 * The plane is returned in the following way:
 *     - plane's normal vector: -returned_vector
 *     - point on plane: v+returned_vector
 */
boost::shared_ptr<Vector3d> separate_point_from_points(const Vector3d& v, const std::vector<Vector3d>& w);


#endif /* POINTS_SEPPLANE_H_ */

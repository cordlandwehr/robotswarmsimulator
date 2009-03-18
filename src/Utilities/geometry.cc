/*
 * Geometry.cpp
 *
 *  Created on: Mar 18, 2009
 *      Author: phoenixx
 */

#include "geometry.h"
#include <vector>
#include <math>
#include <algorithm>

bool Geometry::compare_vectors_by_axis (const Vector3d& vec_a, const Vector3d& vec_b, int axis) {
	if (vec_a.get(axis) <= vec_b.get(axis))
		return true;
	else
		return false;
}

bool Geometry::compare_vectors_by_axis_x (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axes (vec_a, vec_b, 0);
}

bool Geometry::compare_vectors_by_axis_y (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axes (vec_a, vec_b, 0);
}

bool Geometry::compare_vectors_by_axis_z (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axes (vec_a, vec_b, 0);
}

bool Geometry::are_points_on_plane(std::vector<Vector3d> point_list) {
	// if less than for points return true:
	if (point_list.size() < 4)
		return true;

	// else compute Hesse-Form and test distances
	// this is the normal vector by cross product
	Vector3d normal = Vector3d(point_list.at(0).get(1)*point_list.at(1).get(2)-point_list.at(0).get(2)*point_list.at(1).get(1),
			point_list.at(0).get(2)*point_list.at(1).get(0)-point_list.at(0).get(0)*point_list.at(1).get(2),
			point_list.at(0).get(0)*point_list.at(1).get(1)-point_list.at(0).get(1)*point_list.at(1).get(0));

	// distance: layer to origin
	double distance =  normal.get(0)*point_list.at(2).get(0) + normal.get(1)*point_list.at(2).get(1) + normal.get(2)*point_list.at(2).get(2);

	// checks if all points are on layer
	for (int i=3; i < point_list.size(); i++) {
		if (math::fabs( normal.get(0)*point_list.at(i).get(0) + normal.get(1)*point_list.at(i).get(1) + normal.get(2)*point_list.at(i).get(2) - distance)
				> Geometry::kEpsilon)
			return false;
	}

	// thus all points are at the layer
	return true;
}

bool Geometry::is_point_in_smalles_bbox(std::vector<Vector3d> point_list, const Vector3d& testpoint) {
	vector<Vector3d>::iterator iter;

	std::sort (point_list.begin(), point_list.end(), Geometry::compare_vectors_by_axis_x);
	if (testpoint.get(0) < point_list.begin().get(0) || testpoint.get(0) > point_list.end().get(0))
		return false;

	std::sort (point_list.begin(), point_list.end(), Geometry::compare_vectors_by_axis_y);
	if (testpoint.get(1) < point_list.begin().get(1) || testpoint.get(1) > point_list.end().get(1))
		return false;

	std::sort (point_list.begin(), point_list.end(), Geometry::compare_vectors_by_axis_z);
	if (testpoint.get(2) < point_list.begin().get(2) || testpoint.get(2) > point_list.end().get(2))
		return false;

	return true;
}



Geometry::Geometry() {
	// TODO Auto-generated constructor stub

}

Geometry::~Geometry() {
	// TODO Auto-generated destructor stub
}

/*
 * Geometry.cpp
 *
 *  Created on: Mar 18, 2009
 *      Author: phoenixx
 */

#include "geometry.h"
#include <vector>
#include <cmath>
#include <algorithm>

const double Geometry::kEpsilon = 0.000000001;


bool Geometry::compare_vectors_by_axis (const Vector3d& vec_a, const Vector3d& vec_b, int axis) {
	return vec_a(axis) <= vec_b(axis));
}

bool Geometry::compare_vectors_by_axis_x (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axis (vec_a, vec_b, 0);
}

bool Geometry::compare_vectors_by_axis_y (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axis (vec_a, vec_b, 0);
}

bool Geometry::compare_vectors_by_axis_z (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axis (vec_a, vec_b, 0);
}

bool Geometry::are_points_on_plane(std::vector<Vector3d> point_list) {
	// if less than for points return true:
	if (point_list.size() < 4)
		return true;

	// else compute Hesse-Form and test distances
	// this is the normal vector by cross product
	Vector3d normal;
	normal(0) = point_list.at(0)(1)*point_list.at(1)(2)-point_list.at(0)(2)*point_list.at(1)(1);
	normal(1) =	point_list.at(0)(2)*point_list.at(1)(0)-point_list.at(0)(0)*point_list.at(1)(2);
	normal(2) = point_list.at(0)(0)*point_list.at(1)(1)-point_list.at(0)(1)*point_list.at(1)(0);

	// distance: layer to origin
	double distance =  normal(0)*point_list.at(2)(0) + normal(1)*point_list.at(2)(1) + normal(2)*point_list.at(2)(2);

	// checks if all points are on layer
	for (unsigned i=3; i < point_list.size(); i++) {
		if (std::fabs( normal(0)*point_list.at(i)(0) + normal(1)*point_list.at(i)(1) + normal(2)*point_list.at(i)(2) - distance)
				> Geometry::kEpsilon)
			return false;
	}

	// thus all points are at the layer
	return true;
}

bool Geometry::is_point_in_smalles_bbox(std::vector<Vector3d> point_list, const Vector3d& testpoint) {
	std::vector<Vector3d>::iterator iter;

	std::sort (point_list.begin(), point_list.end(), Geometry::compare_vectors_by_axis_x);
	if (testpoint(0) < (*point_list.begin())(0) || testpoint(0) > (*point_list.end())(0))
		return false;

	std::sort (point_list.begin(), point_list.end(), Geometry::compare_vectors_by_axis_y);
	if (testpoint(1) < (*point_list.begin())(1) || testpoint(1) > (*point_list.end())(1))
		return false;

	std::sort (point_list.begin(), point_list.end(), Geometry::compare_vectors_by_axis_z);
	if (testpoint(2) < (*point_list.begin())(2) || testpoint(2) > (*point_list.end())(2))
		return false;

	return true;
}



Geometry::Geometry() {
	// TODO Auto-generated constructor stub

}

Geometry::~Geometry() {
	// TODO Auto-generated destructor stub
}

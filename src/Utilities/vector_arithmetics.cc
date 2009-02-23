/*
 *  vector_arithmetics.cc
 *
 *  Created on: Feb 6, 2009
 *      Author: peter
 */

#include "vector_arithmetics.h"

#include <cmath>
#include <iostream>

double det_matrix3d(const Matrix3d& matrix) {
	double det =   matrix(0,0) * (matrix(1,1)*matrix(2,2) - matrix(2,1)*matrix(1,2))
	             - matrix(1,0) * (matrix(0,1)*matrix(2,2) - matrix(2,1)*matrix(0,2))
	             + matrix(2,0) * (matrix(0,1)*matrix(1,2) - matrix(1,1)*matrix(0,2));
	return det;
}

Matrix3d invert_matrix3d(const Matrix3d& matrix) {
	double det = det_matrix3d(matrix);
	if (det == 0.)
		throw std::invalid_argument("got non-invertible matrix");

	Matrix3d inverse(3, 3);
	inverse(0,0) =   matrix(1,1)*matrix(2,2) - matrix(2,1)*matrix(1,2);
	inverse(0,1) = -(matrix(0,1)*matrix(2,2) - matrix(2,1)*matrix(0,2));
	inverse(0,2) =   matrix(0,1)*matrix(1,2) - matrix(1,1)*matrix(0,2);
	inverse(1,0) = -(matrix(1,0)*matrix(2,2) - matrix(2,0)*matrix(1,2));
	inverse(1,1) =   matrix(0,0)*matrix(2,2) - matrix(2,0)*matrix(0,2);
	inverse(1,2) = -(matrix(0,0)*matrix(1,2) - matrix(1,0)*matrix(0,2));
	inverse(2,0) =   matrix(1,0)*matrix(2,1) - matrix(2,0)*matrix(1,1);
	inverse(2,1) = -(matrix(0,0)*matrix(2,1) - matrix(2,0)*matrix(0,1));
	inverse(2,2) =   matrix(0,0)*matrix(1,1) - matrix(1,0)*matrix(0,1);
	inverse /= det;
	return inverse;
}

double vector3d_distance(const Vector3d& a, const Vector3d& b) {
	double dx = a[0]-b[0];
	double dy = a[1]-b[1];
	double dz = a[2]-b[2];
	return sqrt(dx*dx+dy*dy+dz*dz);
}

void vector3d_set(Vector3d& a, const Vector3d& b) {
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}


bool vector3d_linear_independent(const Vector3d& a, const Vector3d& b, const Vector3d& c) {
	// computation by rule of Sarrus
	double det = a[0]*b[1]*c[2] + a[1]*b[2]*c[0] + a[2]*b[0]*c[1] - a[0]*b[2]*c[1] - a[1]*b[0]*c[2] - a[2]*b[1]*c[0];

	if (det < 0.000000001  &&  det > -0.000000001)	// numerical error
		return false;
	return true;
}


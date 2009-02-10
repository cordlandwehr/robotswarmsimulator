/*
 *  vector_arithmetics.cc
 *
 *  Created on: Feb 6, 2009
 *      Author: peter
 */

#include "vector_arithmetics.h"


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


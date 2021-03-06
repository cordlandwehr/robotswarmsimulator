/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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
 *  vector_arithmetics.h
 *
 *  Created on: Feb 6, 2009
 *      Author: peter
 */

#ifndef VECTOR_ARITHMETICS_H
#define VECTOR_ARITHMETICS_H

#include <string>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/blas.hpp>


/**
 * constants for better readability of vector arithmetic
 */
const int kXCoord = 0;
const int kYCoord = 1;
const int kZCoord = 2;

/**
 * \brief Support class for three-dimensional vectors.
 *
 * This is a convenient typedef for the vector type of boost::ublas.
 * \see http://www.boost.org/doc/libs/1_37_0/libs/numeric/ublas/doc/vector.htm#vector
 */
typedef boost::numeric::ublas::c_vector<double, 3> Vector3d;

/**
 * Elementwise comparisson of three-dimensional vectors.
 */
bool operator==(const Vector3d& v, const Vector3d& w);

/**
 * Elementwise comparisson of three-dimensional vectors.
 */
bool operator!=(const Vector3d& v, const Vector3d& w);


/**
 * \brief Support class for 3x3 matrices.
 *
 * This is a convenient typedef for the matrix tye of boost::ublas
 * \see http://www.boost.org/doc/libs/1_37_0/libs/numeric/ublas/doc/matrix.htm
 */
typedef boost::numeric::ublas::c_matrix<double, 3, 3> Matrix3d;

/**
 * \brief Computes the determinant of the given 3x3 matrix.
 */
double det_matrix3d(const Matrix3d& matrix);

/**
 * \brief Returns the inverse of the given 3x3 matrix.
 *
 * Throws an exception if given matrix is not invertible.
 */
Matrix3d invert_matrix3d(const Matrix3d& matrix);

/**
 * \return euclidean distance between a and b
 */
double vector3d_distance(const Vector3d& a, const Vector3d& b);

/**
 * \return p-norm distance between a and b
 */
double vector3d_distance(const Vector3d& a, const Vector3d& b, int p);

/**
 * \return sets xyz of a to the values of b
 */
void vector3d_set(Vector3d& a, const Vector3d& b);

/**
 * \return "true" if the given vectors are linear independent
 */
bool vector3d_linear_independent(const Vector3d& a, const Vector3d& b, const Vector3d& c);

Vector3d string_to_vec(std::string  str);

/**
 * \return p-norm of vector
 */
double vector3d_get_length(Vector3d vec, int p);

/**
 * normalizes the given vector (iff vector3d_get_length(Vector3d vec, 2) != 0)
 */
void vector3d_normalize(Vector3d& vec);

/**
 * sets the given vector to length len
 */
void vector3d_set_length(Vector3d& vec, double len);

/**
 * sets the given vector to length len, iff it is longer than len
 */
void vector3d_set_maxlength(Vector3d& vec, double len);

/**
 * rotates the given vector around x-axis by rx and around y-axis by ry and around z-axis by rz.
 * angles are specified in rad.
 */
void vector3d_rotate(Vector3d & tp, double rx, double ry, double rz);

/**
 * interpolates between two vectors by the given factor.
 */
Vector3d vector3d_interpolate(Vector3d & vec1, Vector3d & vec2, double factor);

/**
 * calculates the cross product of the given vectors.
 */
Vector3d vector3d_cross(const Vector3d & vector1, const Vector3d & vector2);

/**
 * multiplies the the given vectors component wise.
 */
Vector3d vector3d_mul(const Vector3d & vector1, const Vector3d & vector2);

#endif /* VECTOR_ARITHMETICS_H */

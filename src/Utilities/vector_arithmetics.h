/*
 *  vector_arithmetics.h
 *
 *  Created on: Feb 6, 2009
 *      Author: peter
 */

#ifndef VECTOR_ARITHMETICS_H
#define VECTOR_ARITHMETICS_H

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>


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
 * \return sets xyz of a to the values of b
 */
void vector3d_set(Vector3d& a, const Vector3d& b);

#endif /* VECTOR_ARITHMETICS_H */

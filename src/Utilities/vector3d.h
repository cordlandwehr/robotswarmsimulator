/*
 * vector3d.h
 *
 *  Created on: Jan 8, 2009
 *      Author: craupach
 */

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

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
// alternative, but probably slower:
//typedef boost::numeric::ublas::bounded_vector<double, 3> Vector3d;

/*
 * The typedef above replaces the following class. Its functionallity is already provided by
 * boost::ublas. It may be, that the boost implementation is significantly slower because it is
 * designed for quite large vectors. But if we realize that vector arithmetic is an efficiency
 * problem, we can still implement our own vector class using the same interface as boost.
 * -peter
 */
///**
// * \brief support class for three-dimensional vectors
// *
// * This is a class and not a struct because there might be a lot of helper functions for it in
// * the future.
// *
// * TODO(craupach): add helper functions for adding, subtracting vectors etc.
// */
//class Vector3d {
//
//public:
//	Vector3d(double x, double y, double z): x_(x), y_(y), z_(z) {}
//
//	double x() const {return x_;}
//	double y() const {return y_;}
//	double z() const {return z_;}
//
//	void set_x(double x) {x_ = x;}
//	void set_y(double y) {y_ = y;}
//	void set_z(double z) {z_ = z;}
//
//private:
//	double x_;
//	double y_;
//	double z_;
//};

#endif /* VECTOR3D_H_ */

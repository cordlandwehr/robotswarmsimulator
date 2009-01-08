/*
 * vector3d.h
 *
 *  Created on: Jan 8, 2009
 *      Author: craupach
 */

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

/**
 * \brief support class for three-dimensional vectors
 *
 * This is a class and not a struct because there might be a lot of helper functions for it in
 * the future.
 *
 * TODO(craupach): add helper functions for adding, subtracting vectors etc.
 */
class Vector3d {

public:
	Vector3d(double x, double y, double z): x_(x), y_(y), z_(z) {}

	double x() const {return x_;}
	double y() const {return y_;}
	double z() const {return z_;}

	void set_x(double x) {x_ = x;}
	void set_y(double y) {y_ = y;}
	void set_z(double z) {z_ = z;}

private:
	double x_;
	double y_;
	double z_;
};

#endif /* VECTOR3D_H_ */

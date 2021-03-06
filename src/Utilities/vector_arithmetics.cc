/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 *  vector_arithmetics.cc
 *
 *  Created on: Feb 6, 2009
 *      Author: peter
 */

#include "vector_arithmetics.h"

#include <cmath>
#include <iostream>
#include <sstream>

bool operator==(const Vector3d& v, const Vector3d& w) {
    return v(0) == w(0) && v(1) == w(1) && v(2) == w(2);
}

bool operator!=(const Vector3d& v, const Vector3d& w) {
    return v(0) != w(0) || v(1) != w(1) || v(2) != w(2);
}

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


double vector3d_distance(const Vector3d& a, const Vector3d& b, int p) {
	double dx = a[0]-b[0];
	double dy = a[1]-b[1];
	double dz = a[2]-b[2];
	return pow( pow(dx,p) + pow(dy,p) + pow(dz,p) , 1.0/p);
}


double vector3d_distance(const Vector3d& a, const Vector3d& b) {
	return vector3d_distance(a,b,2);
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


Vector3d string_to_vec(std::string str ){
	Vector3d vec;
	char c;

	std::istringstream string_stream( str );
	string_stream >> vec(0) >> c >> vec(1) >> c >> vec(2);
	return vec;
}


double vector3d_get_length(Vector3d vec, int p) {
	return pow( pow(vec(0),p) + pow(vec(1),p) + pow(vec(2),p) , 1.0/p);
}

void vector3d_normalize(Vector3d& vec) {
	double len = vector3d_get_length(vec, 2);
	if (len != 0) {
		vec /= len;
	}
}

void vector3d_set_length(Vector3d& vec, double len) {
	vector3d_normalize(vec);
	vec(0) *= len;
	vec(1) *= len;
	vec(2) *= len;
}

void vector3d_set_maxlength(Vector3d& vec, double len) {
	double isLen = pow( pow(vec(0),2) + pow(vec(1),2) + pow(vec(2),2) , 0.5);
	if(isLen > len)
		vector3d_set_length(vec, len);
}

void vector3d_rotate(Vector3d & tp, double rx, double ry, double rz) {
	double x,y,z;
	double newX, newY, newZ;

	x = tp[0];
	y = tp[1];
	z = tp[2];

	newX = cos(rx)*cos(rz)*x - cos(ry)*sin(rz)*y + sin(ry)*z;
	newY = (cos(rx)*sin(rz)+sin(rx)*sin(ry)*cos(rz))*x;
	newY += (cos(rx)*cos(rz)-sin(rx)*sin(ry)*sin(rz))*y;
	newY -= sin(rx)*cos(ry)*z;
	newZ = (sin(rx)*sin(rz)-cos(rx)*sin(ry)*cos(rz))*x;
	newZ += (sin(rx)*cos(rz)+cos(rx)*sin(ry)*sin(rz))*y;
	newZ += cos(rx)*cos(ry)*z;

	tp[0] = newX;
	tp[1] = newY;
	tp[2] = newZ;
}


Vector3d vector3d_interpolate(Vector3d & vec1, Vector3d & vec2, double factor) {
	Vector3d out;

	out[0] = factor*vec2(0)+(1-factor)*vec1(0);
	out[1] = factor*vec2(1)+(1-factor)*vec1(1);
	out[2] = factor*vec2(2)+(1-factor)*vec1(2);

	return out;
}


Vector3d vector3d_cross(const Vector3d & vector1 ,const Vector3d & vector2) {
	Vector3d normal = Vector3d();

	// Calculate the cross product with the non communitive equation
	normal(0) = vector1(1) * vector2(2) - vector1(2) * vector2(1);
	normal(1) = vector1(2) * vector2(0) - vector1(0) * vector2(2);
	normal(2) = vector1(0) * vector2(1) - vector1(1) * vector2(0);

	// Return the cross product
	return normal;
}

Vector3d vector3d_mul(const Vector3d & vector1, const Vector3d & vector2) {
	Vector3d out;

	out[0] = vector1(0)*vector2(0);
	out[1] = vector1(1)*vector2(1);
	out[2] = vector1(2)*vector2(2);

	return out;
}

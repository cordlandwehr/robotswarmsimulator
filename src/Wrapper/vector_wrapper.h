/*
 * vector_wrapper.h
 *
 *  Created on: Apr 20, 2009
 *      Author: craupach
 *
 *
 *      This file should contain all wrappers related to the vector3dwrapper struct.
 *      Some of the wrappers contained in this file will be transferred to some
 *      computational geometry file in the future. Also some functions will be transfered to a
 *      .cc file.
 */

#ifndef VECTOR_WRAPPER_H_
#define VECTOR_WRAPPER_H_

#include <vector>
#include <iostream> 																													

#include "../Utilities/vector_arithmetics.h"


namespace LuaWrapper {

/**
 * Wrapper for Vector3d. Allows lua scripts to work with Vector3d objects.
 * @see Vector3d
 */
struct Vector3dWrapper {
	Vector3dWrapper() : x(0), y(0), z(0) {;}
	Vector3dWrapper(double x, double y, double z) : x(x), y(y), z(z) {;}

	const Vector3dWrapper operator+(const Vector3dWrapper& rhs) const {
		return Vector3dWrapper(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	const Vector3dWrapper operator-(const Vector3dWrapper& rhs) const {
		return Vector3dWrapper(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	const Vector3dWrapper operator*(const Vector3dWrapper& rhs) const {
		return Vector3dWrapper(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	friend const Vector3dWrapper operator*(double scalar, const Vector3dWrapper& vec) {
		return vec*scalar;
	}

	const Vector3dWrapper operator/(double div) const {
		return Vector3dWrapper(x / div, y / div, z / div);
	}

	const Vector3dWrapper operator*(double scalar) const {
		return Vector3dWrapper(x * scalar, y * scalar, z * scalar);
	}

	const bool operator==(const Vector3dWrapper& rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector3dWrapper& rhs) {
		//Note: not implemented using operator<< of Vector3d, because latter is ugly.
		return os << "{x = " << rhs.x << ", y = " << rhs.y << ", z = " << rhs.z << "}";
	}


	double x;
	double y;
	double z;

};

/**
 * transforms a vector wrapper into a vector
 */
const Vector3d transform(const LuaWrapper::Vector3dWrapper& vec);

/**
 * transforms a vector of vector wrappers into a vector of vectors
 */
const std::vector<Vector3d> transform(const std::vector<LuaWrapper::Vector3dWrapper>& vec);


const LuaWrapper::Vector3dWrapper transform(const Vector3d& vec);

const std::vector<LuaWrapper::Vector3dWrapper> transform(const std::vector<Vector3d>& vec);




}


#endif /* VECTOR_WRAPPER_H_ */

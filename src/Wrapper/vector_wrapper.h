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
 * transformas a vector wrapper into a vector
 */
const Vector3d transform(const LuaWrapper::Vector3dWrapper& vec) {
	Vector3d result;
	result.insert_element(kXCoord, vec.x);
	result.insert_element(kYCoord, vec.y);
	result.insert_element(kZCoord, vec.z);
	return result;
}

/**
 * transforms a vector of vector wrappers into a vector of vectors
 */
const std::vector<Vector3d> transform(const std::vector<LuaWrapper::Vector3dWrapper>& vec) {
	std::vector<Vector3d> result;
	result.resize(vec.size());
	std::transform(vec.begin(), vec.end(), result.begin(), boost::bind(static_cast<const Vector3d(*)(const LuaWrapper::Vector3dWrapper&)>(&transform),_1));
	return result;
}


const LuaWrapper::Vector3dWrapper transform(const Vector3d& vec) {
	return LuaWrapper::Vector3dWrapper(vec(0), vec(1), vec(2));
}

const std::vector<LuaWrapper::Vector3dWrapper> transform(const std::vector<Vector3d>& vec) {
	std::vector<LuaWrapper::Vector3dWrapper> result;
	result.resize(vec.size());
	std::transform(vec.begin(), vec.end(), result.begin(), boost::bind(static_cast<const LuaWrapper::Vector3dWrapper(*)(const Vector3d&)>(&transform),_1));
	return result;
}


/**
 * Computes distance between two Vectors
 * @param v is the first vector
 * @param w is the second vector
 * @return distance
 */
const double compute_distance(const LuaWrapper::Vector3dWrapper& v, const LuaWrapper::Vector3dWrapper& w) {
	return vector3d_distance(transform(v), transform(w));
}

/**
 * Computes distance between two Vectors by p-norm
 * @param v is the first vector
 * @param w is the second vector
 * @param p is value of norm
 * @return distance
 */
const double compute_distance(const LuaWrapper::Vector3dWrapper& v, const LuaWrapper::Vector3dWrapper& w, int p) {
	return vector3d_distance(transform(v), transform(w), p);
}

/**
 * Computes center of given points.
 * @param point_list is vector of points
 * @return the center of gravity
 */
const LuaWrapper::Vector3dWrapper compute_COG(std::vector<LuaWrapper::Vector3dWrapper> point_list) {
	return transform(PointAlgorithms::compute_COG(transform(point_list)));
}

/**
 * Computes center of the miniball around the given points.
 * @param point_list is vector of points
 * @return the center of the miniball around the given points
 */
const LuaWrapper::Vector3dWrapper compute_CMinBall(std::vector<LuaWrapper::Vector3dWrapper> point_list) {
	return transform(PointAlgorithms::compute_CMinBall(transform(point_list)));
}

/**
 * Sort vectors by euclidean norm, distance to zero
 * @return sorted point_list
 */
const std::vector<LuaWrapper::Vector3dWrapper> sort_points_by_distance(std::vector<LuaWrapper::Vector3dWrapper> point_list) {
	std::vector<Vector3d> vec = transform(point_list);
	MiscAlgorithms::sort_points_by_distance(vec, 2);
	return transform(vec);
}

/**
 * Wrapper around
 */
const LuaWrapper::Vector3dWrapper separate_point_from_points(const LuaWrapper::Vector3dWrapper& v, const std::vector<LuaWrapper::Vector3dWrapper>& w) {
    return transform(::separate_point_from_points(transform(v), transform(w)));
}

const LuaWrapper::Vector3dWrapper compute_cog_of_ch_of_points(const std::vector<LuaWrapper::Vector3dWrapper>& points) {
    return transform(CHAlgorithms::compute_cog_of_ch_of_points(transform(points)));
}

const bool point_contained_in_convex_hull_of_points(const LuaWrapper::Vector3dWrapper& point, const std::vector<LuaWrapper::Vector3dWrapper> points) {
	return CHAlgorithms::point_contained_in_convex_hull_of_points(transform(point), transform(points));
}

const LuaWrapper::Vector3dWrapper random_point_in_ch(const std::vector<LuaWrapper::Vector3dWrapper> points, int seed) {
	return transform(CHAlgorithms::random_point_in_ch(transform(points), seed));
}


}


#endif /* VECTOR_WRAPPER_H_ */

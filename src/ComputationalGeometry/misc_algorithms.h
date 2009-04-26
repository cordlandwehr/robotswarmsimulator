/*
 * misc_algorithms.h
 *
 *  Created on: Mar 18, 2009
 *      Author: phoenixx
 *
 * \brief
 * This class provides useful functions for computational geometry. E.g. checks if point is
 * inside of of a polygon/polyhedron, computes convex hull ...
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <utility>

#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

#include <Utilities/vector_arithmetics.h>


class MiscAlgorithms {
public:
	/**
	 * Compares two Vector3ds a and b on given axis. Returns true iff a.axis <= b.axis.
	 * @param vector a
	 * @param vector b
	 * @param axis
	 * @return true iff a <= b
	 */
	static bool compare_vectors_by_axis (const Vector3d& vec_a, const Vector3d& vec_b, int axis);

	static bool compare_vectors_by_axis_x (const Vector3d& vec_a, const Vector3d& vec_b);

	static bool compare_vectors_by_axis_y (const Vector3d& vec_a, const Vector3d& vec_b);

	static bool compare_vectors_by_axis_z (const Vector3d& vec_a, const Vector3d& vec_b);

	/**
	 * Tests if all points are on exactly one plane.
	 */
	static bool are_points_on_plane(std::vector<Vector3d> point_list);

	//TODO
	static bool is_point_in_poly_3D(const Vector3d& a, const Vector3d& b);

	/**
	 * Tests if the given point is inside a bounding box defined by point set.
	 * @param points that define the box
	 * @param point the shall be tested to be inside of this box
	 * @return true or false
	 */
	static bool is_point_in_smallest_bbox(std::vector<Vector3d> point_list, const Vector3d& testpoint);

	/**
	 * \return sorts given vector of Vector3ds by euclidean distance from origin
	 */
	static void sort_points_by_distance(std::vector<Vector3d>& point_list, int p);

	/**
	 * \return sorts given vector of indices for Vector3ds by euclidean distance from origin
	 */
	static void sort_pointslist_by_distance(std::vector< std::pair<Vector3d,std::size_t> >& point_list, int p);
	
	
	/**
	 * \brief Calculates the shim plane for the given set of points.
	 */
	static boost::tuple<Vector3d,Vector3d> calculate_shim_plane(const std::vector<Vector3d>& pos);


private:
	static const double kEpsilon;	// accuracy value
};

#endif /* GEOMETRY_H_ */

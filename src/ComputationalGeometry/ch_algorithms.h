/*
 * ch_algorithms.h
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 *


/**
 * \class CHAlgorithms
 *
 * This class will provide some useful algorithms working with a convex hull.
 *
 */

#ifndef CH_ALGORITHMS_H_
#define CH_ALGORITHMS_H_

#include <boost/smart_ptr.hpp>
#include <vector>

//CGAL includes
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/copy_n.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Convex_hull_traits_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/MP_Float.h>

#include "../Utilities/vector_arithmetics.h"

//CGAL typedefs
typedef CGAL::MP_Float 								RT;
typedef CGAL::Homogeneous<RT>						K;
typedef CGAL::Convex_hull_traits_3<K>				Traits;
typedef Traits::Polyhedron_3						Polyhedron_3;
typedef K::Segment_3								Segment_3;
typedef K::Point_3									Point_3;
typedef K::Point_2									Point_2;
typedef CGAL::Creator_uniform_3<RT, Point_3>		PointCreator;

class CHAlgorithms {

	friend class point_contained_in_ch_polyhedron_test;
	friend class point_contained_in_ch_segment_test;
	friend class point_contained_in_ch_point_test;

public:

	/**
	 * This method computes the convex hull of the given points.
	 * \param points	Points whose convex hull to compute.
	 * \return 			Object containing the convex hull of the given points.
	 */
	static CGAL::Object compute_convex_hull_3d(std::vector<Vector3d> points);

	/**
	 * This method computes the COG of the given polyhedron.
	 * \param poly		Polyhedron of which to compute COG
	 * \return			COG of the given polyhedron
	 */
	static Vector3d compute_cog_of_polyhedron(Polyhedron_3 poly);

	/**
	 * This method computes the COG of the given segment.
	 * I.e. this mehtod only computes the center of the endpoints of the given segment.
	 * \param seg			Segment which COG to compute.
	 * \return				Center of given segment segment.
	 */
	static Vector3d compute_cog_of_segment(Segment_3 seg);

	/**
	 * This method computes the COG of the convex hull of the given set of points.
	 * \param points	Set of points of which to compute CH
	 * \return			COG of the convex hull of the given set of points.
	 */
	static Vector3d compute_cog_of_ch_of_points(std::vector<Vector3d> points);

	/**
	 * This method checks whether the given point is contained in the given convex hull of the given points.
	 * \param point		Point to check.
	 * \param points	Points of whicht to compute convex hull.
	 * \return			true, if the given point is contained in the convex hull of the given points, false otherwise.
	 */
	static bool point_contained_in_convex_hull_of_points(Vector3d point, std::vector<Vector3d> points);

	/**
	 * This method transforms the given Vector3d to a Point_3.
	 * \param point		Vector3d to transform to a Point_3
	 * \return 			Point_3 of the given Vector3d
	 * \see 			point_3_to_vector3d
	 */
	static Point_3 vector3d_to_point_3(Vector3d point);

	/**
	 * This method transforms the given Point_3 to a Vector3d.
	 * \param point		Point_3 to transform to a Vector3d
	 * \return 			Vector3d of the given Point_3
	 * \see 			vector3d_to_point_3
	 */
	static Vector3d point_3_to_vector3d(Point_3 point_3);

	/**
	 * This method prints the vertices of the given convex hull.
	 * \param ch	Convex hull which vertices to print.
	 */
	static void print_vertices_of_ch(CGAL::Object ch);

	/**
	 * This method prints the endpoints of the given segment.
	 * \param seg	Segment whose endpoints to print.
	 */
	static void print_vertices_of_segment(Segment_3 seg);

	/**
	 * This method prints the vertices of the given polyhedron.
	 * \param poly	Polyhedron whose vertices to print.
	 */
	static void print_vertices_of_polyhedron(Polyhedron_3 poly);

	/**
	 * This method returns a point chosen uniformly at random from the convex hull of the given points
	 * \param points	Points whose convex hull to compute from which to choose random point.
	 * \return 			Point chosen chosen uniformly at random from the convex hull of the given points.
	 */
	static Vector3d random_point_in_ch(std::vector<Vector3d> points);

};

#endif /* CH_ALGORITHMS_H_ */

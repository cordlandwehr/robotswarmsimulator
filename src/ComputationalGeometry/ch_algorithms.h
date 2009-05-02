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

#include <CGAL/point_generators_3.h>
#include <CGAL/copy_n.h>
#include <CGAL/Convex_hull_traits_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Simple_cartesian.h>
#include <vector>

#include "../Utilities/vector_arithmetics.h"

#include <CGAL/double.h>
typedef double RT;

typedef CGAL::Simple_cartesian<double>		K;
typedef CGAL::Convex_hull_traits_3<K>		Traits;
typedef Traits::Polyhedron_3				Polyhedron_3;
typedef K::Segment_3						Segment_3;
typedef K::Point_3							Point_3;

// define point creator
typedef CGAL::Creator_uniform_3<double, Point_3>  PointCreator;

class CHAlgorithms {

	friend class ch_test_1;

public:

	/**
	 * This method computes the convex hull of the given points.
	 * \param points	Points whose convex hull to compute.
	 * \return 			Object containing the convex hull of the given points.
	 */
	static CGAL::Object compute_convex_hull_3d(std::vector<Point_3> points);

	/**
	 * This method computes the COG of the given polyhedron.
	 * \param poly		Polyhedron of which to compute COG
	 * \return			COG of the given polyhedron
	 */
	static Vector3d compute_cog_of_polyhedron(CGAL::Object poly);

	/**
	 * This method computes the COG of the convex hull of the given set of points.
	 * \param points	Set of points of which to compute CH
	 * \return			COG of the convex hull of the given set of points.
	 */
	static Vector3d compute_cog_of_ch_of_points(std::vector<Point_3> points);

};

#endif /* CH_ALGORITHMS_H_ */

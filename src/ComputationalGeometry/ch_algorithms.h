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

#include <CGAL/Homogeneous.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/copy_n.h>
#include <CGAL/Convex_hull_traits_3.h>
#include <CGAL/convex_hull_3.h>
#include <vector>

//#ifdef CGAL_USE_GMP
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz RT;
//#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float RT;
//#endif

typedef CGAL::Homogeneous<RT> K;
typedef CGAL::Convex_hull_traits_3<K>             Traits;
typedef Traits::Polyhedron_3                      Polyhedron_3;
typedef K::Segment_3                              Segment_3;
typedef K::Point_3								Point_3;

// define point creator
typedef CGAL::Creator_uniform_3<double, Point_3>  PointCreator;

class CHAlgorithms {

	friend class ch_test_1;

public:
	CHAlgorithms();
	virtual ~CHAlgorithms();

	/**
	 * This method computes the convex hull of the given points.
	 * \param points	Points whose convex hull to compute.
	 * \return 			Object containing the convex hull of the given points.
	 */
	CGAL::Object compute_convex_hull_3d(std::vector<Point_3> points);

};

#endif /* CH_ALGORITHMS_H_ */

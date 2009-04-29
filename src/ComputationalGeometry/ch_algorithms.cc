/*
 * ch_algorithms.cc
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 */

#include "ch_algorithms.h"

CHAlgorithms::CHAlgorithms() {
	// TODO Auto-generated constructor stub

}

CHAlgorithms::~CHAlgorithms() {
	// TODO Auto-generated destructor stub
}

CGAL::Object CHAlgorithms::compute_convex_hull_3d(std::vector<Point_3> points) {
	// define object to hold convex hull
	CGAL::Object ch_object;

	//compute convex hull
	CGAL::convex_hull_3(points.begin(), points.end(), ch_object);

	return ch_object;
}

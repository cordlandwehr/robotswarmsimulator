/*
 * ch_algorithms.cc
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 */

#include "ch_algorithms.h"
#include "../Utilities/unsupported_operation_exception.h"

CGAL::Object CHAlgorithms::compute_convex_hull_3d(std::vector<Point_3> points) {
	// define object to hold convex hull
	CGAL::Object ch_object;

	//compute convex hull
	CGAL::convex_hull_3(points.begin(), points.end(), ch_object);

	return ch_object;
}

Vector3d CHAlgorithms::compute_cog_of_polyhedron(CGAL::Object poly) {
	Vector3d cog;
	cog(0) = 0;
	cog(1) = 0;
	cog(2) = 0;

	Polyhedron_3 polyhedron;
	Point_3 p;
    int num = 0;

	// determine what kind of object it is
	if ( CGAL::assign (polyhedron, poly) ) {
	    for ( Polyhedron_3::Vertex_iterator v = polyhedron.vertices_begin(); v != polyhedron.vertices_end(); ++v) {
	    	p = v->point();
	    	cog(0) += p.x();
	    	cog(1) += p.y();
	    	cog(2) += p.z();
	    	num++;
	    }
	    cog /= num;
	}
	else {
		throw UnsupportedOperationException("Given CGAL-object isn't a polyhedron.");
	}

	return cog;
}

Vector3d CHAlgorithms::compute_cog_of_ch_of_points(std::vector<Point_3> points) {

	// define object to hold convex hull
	CGAL::Object ch_object = compute_convex_hull_3d(points);
	Vector3d cog = compute_cog_of_polyhedron(ch_object);

	return cog;
}

/*
 * ch_algorithms.cc
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 */

#include "ch_algorithms.h"

CGAL::Object CHAlgorithms::compute_convex_hull_3d(std::vector<Point_3> points) {
	// define object to hold convex hull
	CGAL::Object ch_object;

	//compute convex hull
	CGAL::convex_hull_3(points.begin(), points.end(), ch_object);

	return ch_object;
}

Vector3d CHAlgorithms::compute_cog_of_ch(CGAL::Object ch) {
	Vector3d cog;
	cog(0) = 0;
	cog(1) = 0;
	cog(2) = 0;

	Polyhedron_3 polyhedron;
	Segment_3 segment;
	Point_3 p;
    int num = 0;

	// determine what kind of object it is
	if (CGAL::assign(segment, ch) ) {
		//TODO
	}
	else if ( CGAL::assign (polyhedron, ch) ) {
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
		//error
		std::cout << "convex hull error!" << std::endl;
	}

	return cog;
}

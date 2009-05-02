/*
 * ch_algorithms.cc
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 */

#include "ch_algorithms.h"
#include "../Utilities/unsupported_operation_exception.h"


CGAL::Object CHAlgorithms::compute_convex_hull_3d(std::vector<Vector3d> points) {

	//create Point_3 out of Vector3d
	std::vector<Point_3> points_3;
	Vector3d cur_point;
	for(int i=0; i<points.size(); i++) {
		cur_point = points[i];
		Point_3 p(cur_point(0), cur_point(1), cur_point(2));
		points_3.push_back(p);
	}

	// define object to hold convex hull
	CGAL::Object ch_object;

	//compute convex hull
	CGAL::convex_hull_3(points_3.begin(), points_3.end(), ch_object);

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

Vector3d CHAlgorithms::compute_cog_of_ch_of_points(std::vector<Vector3d> points) {

	// define object to hold convex hull
	CGAL::Object ch_object = compute_convex_hull_3d(points);
	Vector3d cog = compute_cog_of_polyhedron(ch_object);

	return cog;
}

bool CHAlgorithms::point_contained_in_convex_hull_of_points(Vector3d point, std::vector<Vector3d> points) {

	Polyhedron_3 poly1, poly2;
	CGAL::Object ch1, ch2;
	Point_3 p;

	//compute convex hull of the given points
	ch1 = compute_convex_hull_3d(points);

	//compute convex hull of the given points including point to check
	std::vector<Vector3d> new_points = points;
	new_points.push_back(point);
	ch2 = compute_convex_hull_3d(points);

	//transform convex hull to polyhedrons
	if( !CGAL::assign(poly1, ch1) || !CGAL::assign(poly2, ch2) ) {
		throw UnsupportedOperationException("Given CGAL-object isn't a polyhedron.");
	}

	//Check if convex hull has been modified by adding this point.
	//If it hasn't, then the given point is inside the convex hull of the given points.
	//To do this, check if each vertex of poly2 is a vertex of poly1
	//TODO(martinah) Maybe use a more efficient way to check this.
	bool vertex_exists;
    for ( Polyhedron_3::Vertex_iterator u = poly2.vertices_begin(); u != poly2.vertices_end(); ++u) {
    	p = u->point();
    	//check if p is a vertex of poly1
    	vertex_exists = false;
    	for ( Polyhedron_3::Vertex_iterator v = poly1.vertices_begin(); v != poly1.vertices_end(); ++v) {
    		if( p == v->point() )
    			vertex_exists = true;
    	}
    	if(!vertex_exists) {
    		//p isn't a vertex of poly1 => given point is not contained in the CH of the given points
    		return false;
    	}
    }

	return true;
}

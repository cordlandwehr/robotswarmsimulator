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
	Vector3d tmp_point;
	for(int i=0; i<points.size(); i++) {
		points_3.push_back(vector3d_to_point_3(points[i]));
	}

	// define object to hold convex hull
	CGAL::Object ch_object;
	//compute convex hull
	CGAL::convex_hull_3(points_3.begin(), points_3.end(), ch_object);

	return ch_object;

}

void CHAlgorithms::print_vertices_of_ch(CGAL::Object ch) {
	Polyhedron_3 poly;

	//determine type of given convex hull
	int num = 0;
	if ( CGAL::assign (poly, ch) ) {
	    for ( Polyhedron_3::Vertex_iterator v = poly.vertices_begin(); v != poly.vertices_end(); ++v) {
	    	std::cout << "Vertex " << num++ << ": " << v->point() << std::endl;
	    }
	}

	//TODO(martinah) implement for other types
}


Vector3d CHAlgorithms::compute_cog_of_polyhedron(CGAL::Object obj) {
	Vector3d cog;
	cog(0) = 0;
	cog(1) = 0;
	cog(2) = 0;

	//check if object is a polyhedron
	Polyhedron_3 poly;
	Point_3 p;
    int num = 0;
	if ( CGAL::assign (poly, obj) ) {
	    for ( Polyhedron_3::Vertex_iterator v = poly.vertices_begin(); v != poly.vertices_end(); ++v) {
	    	p = v->point();
	    	cog(0) += CGAL::to_double(p.hx());
	    	cog(1) += CGAL::to_double(p.hy());
	    	cog(2) += CGAL::to_double(p.hz());
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

	CGAL::Object ch_object = compute_convex_hull_3d(points);
	Vector3d cog = compute_cog_of_polyhedron(ch_object);

	return cog;
}

bool CHAlgorithms::point_contained_in_convex_hull_of_points(Vector3d point, std::vector<Vector3d> points) {

	using namespace std;

	Polyhedron_3 poly1, poly2;
	Segment_3 seg1, seg2;
	Point_3 point1, point2;

	CGAL::Object ch1, ch2;
	Point_3 p;

	bool ch1_is_polyhedron = false;
	bool ch2_is_polyhedron = false;
	bool ch1_is_segment = false;
	bool ch2_is_segment = false;
	bool ch1_is_point = false;
	bool ch2_is_point = false;

	bool vertex_exists, invalid_type;

	//compute convex hull of the given points
	ch1 = compute_convex_hull_3d(points);

	//compute convex hull of the given points including point to check
	std::vector<Vector3d> new_points = points;
	new_points.push_back(point);
	ch2 = compute_convex_hull_3d(new_points);

	//determine type of convex hulls
	invalid_type = false;
	if ( CGAL::assign(poly1, ch1) )
		ch1_is_polyhedron = true;
	else if (CGAL::assign(seg1, ch1))
		ch1_is_segment = true;
	else if (CGAL::assign(point1, ch1))
		ch1_is_point = true;
	else
		invalid_type = true;

	if ( CGAL::assign(poly2, ch2) )
		ch2_is_polyhedron = true;
	else if (CGAL::assign(seg2, ch2))
		ch2_is_segment = true;
	else if (CGAL::assign(point2, ch2))
		ch2_is_point = true;
	else
		invalid_type = true;

	/*
	cout << "Types: " << endl;
	cout << "ch1_is_polyhedron = " << ch1_is_polyhedron << endl;
	cout << "ch2_is_polyhedron = " << ch2_is_polyhedron << endl;
	cout << "ch1_is_segment = " << ch1_is_segment << endl;
	cout << "ch2_is_segment = " << ch2_is_segment << endl;
	cout << "ch1_is_point = " << ch1_is_point << endl;
	cout << "ch2_is_point = " << ch2_is_point << endl;
	*/

	if(invalid_type)
		throw UnsupportedOperationException("Type of convex hulls couldn't be determined.");

	//depending on type of convex hulls, compare vertices
	//at this point the convex hull are of the same type
	if ( ch1_is_polyhedron && ch2_is_polyhedron ) {

		//iterate through vertices of convex hull 2
		for ( Polyhedron_3::Vertex_iterator u = poly2.vertices_begin(); u != poly2.vertices_end(); ++u) {
		   	p = u->point();

		   	//check if p is a vertex of convex hull 1
	    	vertex_exists = false;
	    	for ( Polyhedron_3::Vertex_iterator v = poly1.vertices_begin(); v != poly1.vertices_end(); ++v) {
	    		if( p == v->point() ) {
	    			vertex_exists = true;
	    		}
	    	}
	    	if(!vertex_exists) {
	    		//p isn't a vertex of poly1 => given point is not contained in the CH of the given points
	    		return false;
	    	}
		}
		//if this point is reached, each vertex of ch2 is also a vertex of ch1 => convex hulls are equal
		return true;
	}
	else if ( ch1_is_segment && ch2_is_segment ) {

		//compare endpoints of segments
		if( seg1.source() == seg2.source() && seg1.target() == seg2.target() )
			return true;
		else
			return false;


	}
	else if ( ch1_is_point && ch2_is_point ) {

		//ch1 and ch2 are both points => compare points
		if(point1 == point2)
			return true;
		else
			return false;
	}
	else {
		return false;
	}
}

Point_3 CHAlgorithms::vector3d_to_point_3(Vector3d point) {
	Point_3 point_3(point(0), point(1), point(2));
	return point_3;
}


/*
 * ch_algorithms.cc
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 */

#include <iostream>
#include <math.h>
#include "ch_algorithms.h"
#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/distribution_generator.h"


CGAL::Object CHAlgorithms::compute_convex_hull_3d(const std::vector<Vector3d>& points) {

	//create Point_3 out of Vector3d
	std::vector<Point_3> points_3;
	for(std::size_t i=0; i<points.size(); i++) {
		points_3.push_back(vector3d_to_point_3(points[i]));
	}

	// define object to hold convex hull
	CGAL::Object ch_object;

	//compute convex hull
	CGAL::convex_hull_3(points_3.begin(), points_3.end(), ch_object);

	return ch_object;
}

void CHAlgorithms::print_vertices_of_ch(const CGAL::Object& ch) {
	Polyhedron_3 poly;
	Segment_3 seg;
	Point_3 point;

	//determine type of given convex hull
	if ( CGAL::assign (poly, ch) ) {
		print_vertices_of_polyhedron(poly);
	} else if ( CGAL::assign (seg, ch) ) {
		print_vertices_of_segment(seg);
	} else if ( CGAL::assign (point, ch) ) {
		std::cout << point << std::endl;
	}
}

void CHAlgorithms::print_vertices_of_polyhedron(const Polyhedron_3& poly) {
	int num = 0;
	for ( Polyhedron_3::Vertex_const_iterator v = poly.vertices_begin(); v != poly.vertices_end(); ++v) {
	   	std::cout << "Vertex " << num++ << ": " << v->point() << std::endl;
	}
}

void CHAlgorithms::print_vertices_of_segment(const Segment_3& seg) {
   	std::cout << "Source: " << seg.source() << std::endl;
   	std::cout << "Target: " << seg.target() << std::endl;
}


Vector3d CHAlgorithms::compute_cog_of_segment(const Segment_3& seg) {
	Vector3d cog;
	cog = (point_3_to_vector3d(seg.source()) + point_3_to_vector3d(seg.target())) / 2;
	return cog;
}


Vector3d CHAlgorithms::compute_cog_of_polyhedron(const Polyhedron_3& poly) {
	Vector3d cog;
	cog(0) = 0;
	cog(1) = 0;
	cog(2) = 0;

	Point_3 p;
    int num = 0;
    for ( Polyhedron_3::Vertex_const_iterator v = poly.vertices_begin(); v != poly.vertices_end(); ++v) {
    	p = v->point();
    	cog += point_3_to_vector3d(p);
    	num++;
    }
    cog /= num;

	return cog;
}

Vector3d CHAlgorithms::compute_cog_of_ch_of_points(const std::vector<Vector3d>& points) {

	Vector3d cog;
	Polyhedron_3 poly;
	Segment_3 seg;
	Point_3 point;

	CGAL::Object cd = compute_convex_hull_3d(points);

	//Depending on type of convex hull, compute its COG
	if ( CGAL::assign(poly, cd) ) {
		cog = compute_cog_of_polyhedron(poly);
	} else if ( CGAL::assign(seg, cd) ) {
		cog = compute_cog_of_segment(seg);
	} else if ( CGAL::assign(point, cd) ) {
    	cog = point_3_to_vector3d(point);
	} else {
		throw UnsupportedOperationException("Type of convex hull couldn't be determined.");
	}

	return cog;
}


void CHAlgorithms::compute_facet_planes(Polyhedron_3& polyhedron) {
    Polyhedron_3::Facet_iterator facet_it;
    for (facet_it = polyhedron.facets_begin(); facet_it != polyhedron.facets_end(); ++facet_it) {
        Polyhedron_3::Facet::Halfedge_handle edge = facet_it->halfedge();
        facet_it->plane() = Plane_3(edge->vertex()->point(),
                                    edge->next()->vertex()->point(),
                                    edge->next()->next()->vertex()->point());
    }
}

bool CHAlgorithms::point_contained_in_convex_hull_of_points(const Vector3d& point, const std::vector<Vector3d>& points) {

	using namespace std;

	Polyhedron_3 poly1;
	Segment_3 seg1;
	Point_3 point1;

	//convert given point to Point_3
	Point_3 p = vector3d_to_point_3(point);

	//compute convex hull of the given points
	CGAL::Object ch1 = compute_convex_hull_3d(points);

	//get type of convex hull
	if ( CGAL::assign(poly1, ch1) ) {
		//convex hull is a polyhedron
		//iterate over facets of convex hull and check whether the given point is on the same side of all planes

		//compute plane equations of polyhedron's facets
		compute_facet_planes(poly1);

		CGAL::Oriented_side last_orientation = CGAL::ON_ORIENTED_BOUNDARY;
		CGAL::Oriented_side new_orientation;
		Polyhedron_3::Plane_3 plane;

		//compute plane equations of polyhedron's facets
		compute_facet_planes(poly1);

		for(Polyhedron_3::Facet_iterator facet = poly1.facets_begin(); facet!= poly1.facets_end(); ++facet) {

			plane = facet->plane();
			new_orientation = plane.oriented_side(p);

			//check if orientation equals to old orientation
			if( new_orientation != CGAL::ON_ORIENTED_BOUNDARY
				&& last_orientation != CGAL::ON_ORIENTED_BOUNDARY
				&& new_orientation != last_orientation) {
				return false;
			}

			//set last_orientation
			//(will only be set once: will be set at the first time new_orientation isn't on the boundary)
			if(last_orientation == CGAL::ON_ORIENTED_BOUNDARY && new_orientation != CGAL::ON_ORIENTED_BOUNDARY) {
				last_orientation = new_orientation;
			}
		}
		return true;

	} else if ( CGAL::assign(seg1, ch1) ) {
		//convex hull is a segment
		//test if the given point lies on the segment
		return seg1.has_on(p);

	} else if ( CGAL::assign(point1, ch1) ) {
		//convex hull is a point
		//test if given point and convex hull equal
		if(point1 == p) {
			return true;
		}
	} else {
		throw UnsupportedOperationException("Type of convex hulls couldn't be determined.");
	}

	return false;
}

Point_3 CHAlgorithms::vector3d_to_point_3(const Vector3d& point) {
	Point_3 point_3(point(0), point(1), point(2));
	return point_3;
}

Vector3d CHAlgorithms::point_3_to_vector3d(const Point_3& point_3) {
	Vector3d point;
	point(0) = CGAL::to_double(point_3.x());
	point(1) = CGAL::to_double(point_3.y());
	point(2) = CGAL::to_double(point_3.z());
	return point;
}

Vector3d CHAlgorithms::random_point_in_ch(const std::vector<Vector3d>& points, unsigned int seed) {
	Vector3d rnd_point;
	Polyhedron_3 poly;
	Segment_3 seg;
	Point_3 point;

	//compute convex hull
	CGAL::Object ch;
	ch = compute_convex_hull_3d(points);

	//determine type of convex hull
	if (CGAL::assign(poly, ch)) {

		Polyhedron_3 min_box;
		Vector3d rnd_point_in_min_box;

		//determine minimum and maximum x, y and z coordinates
		Polyhedron_3::Vertex_const_iterator u = poly.vertices_begin();
		Vector3d p = point_3_to_vector3d(u->point());
		double min_x = p(0);
		double max_x = p(0);
		double min_y = p(1);
		double max_y = p(1);
		double min_z = p(2);
		double max_z = p(2);

		while(u!=poly.vertices_end()) {

			p = point_3_to_vector3d(u->point());
			if(p(0) < min_x)
				min_x = p(0);
			if(p(0) > max_x)
				max_x = p(0);
			if(p(1) < min_y)
				min_y = p(1);
			if(p(1) > max_y)
				max_y = p(1);
			if(p(2) < min_z)
				min_z = p(2);
			if(p(2) > max_z)
				max_z = p(2);

			++u;
		}
		boost::shared_ptr<DistributionGenerator> dist_gen_x;
		boost::shared_ptr<DistributionGenerator> dist_gen_y;
		boost::shared_ptr<DistributionGenerator> dist_gen_z;
		dist_gen_x.reset(new DistributionGenerator(seed));
		dist_gen_y.reset(new DistributionGenerator(seed+1));
		dist_gen_z.reset(new DistributionGenerator(seed+2));

		dist_gen_x->init_uniform_real(min_x, max_x);
		dist_gen_y->init_uniform_real(min_y, max_y);
		dist_gen_z->init_uniform_real(min_z, max_z);

		do {
			//choose random point in minimum surrounding box
			rnd_point_in_min_box(0) = dist_gen_x->get_value_uniform_real();
			rnd_point_in_min_box(1) = dist_gen_y->get_value_uniform_real();
			rnd_point_in_min_box(2) = dist_gen_z->get_value_uniform_real();

		} while(!point_contained_in_convex_hull_of_points(rnd_point_in_min_box, points));

		rnd_point = rnd_point_in_min_box;

	} else if (CGAL::assign(seg, ch)) {

		//TODO(martinah) implement

	} else if (CGAL::assign(point, ch)) {
		rnd_point = point_3_to_vector3d(point);

	} else {
		throw UnsupportedOperationException("Type of convex hulls couldn't be determined.");
	}

	return rnd_point;
}

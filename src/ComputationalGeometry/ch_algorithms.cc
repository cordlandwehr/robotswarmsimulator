/*
 * ch_algorithms.cc
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 */

#include <iostream>
#include <math.h>
#include <boost/foreach.hpp>
#include "ch_algorithms.h"
#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/distribution_generator.h"


void CHAlgorithms::compute_facet_planes(Polyhedron_3& polyhedron) {
    Polyhedron_3::Facet_iterator facet_it;
    for (facet_it = polyhedron.facets_begin(); facet_it != polyhedron.facets_end(); ++facet_it) {
        Polyhedron_3::Facet::Halfedge_handle edge = facet_it->halfedge();
        facet_it->plane() = Plane_3(edge->vertex()->point(),
                                    edge->next()->vertex()->point(),
                                    edge->next()->next()->vertex()->point());
    }
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


Point_3 CHAlgorithms::compute_cog_of_segment(const Segment_3& seg) {
	return seg.source() + (seg.target() - seg.source())/2.;
}


Point_3 CHAlgorithms::compute_cog_of_polyhedron(const Polyhedron_3& poly) {
	Vector_3 cog(0, 0, 0);
	Point_3 p;
    for ( Polyhedron_3::Vertex_const_iterator v = poly.vertices_begin(); v != poly.vertices_end(); ++v) {
        cog = cog + (v->point() - CGAL::ORIGIN);
    }
	return CGAL::ORIGIN + cog/poly.size_of_vertices();
}


Vector3d CHAlgorithms::compute_cog_of_ch_of_points(const std::vector<Vector3d>& points) {

	Vector3d cog;
	Polyhedron_3 poly;
	Segment_3 seg;
	Point_3 point;

	CGAL::Object ch;
    std::vector<Point_3> cgal_points = transform(points);
    CGAL::convex_hull_3(cgal_points.begin(), cgal_points.end(), ch);

	//Depending on type of convex hull, compute its COG
	if ( CGAL::assign(poly, ch) ) {
		cog = transform(compute_cog_of_polyhedron(poly));
	} else if ( CGAL::assign(seg, ch) ) {
		cog = transform(compute_cog_of_segment(seg));
	} else if ( CGAL::assign(point, ch) ) {
    	cog = transform(point);
	} else {
		throw UnsupportedOperationException("Type of convex hull couldn't be determined.");
	}

	return cog;
}


bool CHAlgorithms::point_contained_in_convex_hull_of_points(const Vector3d& point, const std::vector<Vector3d>& points) {
    return point_contained_in_convex_hull_of_points(transform(point), transform(points));
}

bool CHAlgorithms::point_contained_in_convex_hull_of_points(const Point_3& p, const std::vector<Point_3>& cgal_points) {
    
	Polyhedron_3 poly1;
	Segment_3 seg1;
	Point_3 point1;
    
	//compute convex hull of the given points
	CGAL::Object ch1;
    CGAL::convex_hull_3(cgal_points.begin(), cgal_points.end(), ch1);
    
	//get type of convex hull
	if ( CGAL::assign(poly1, ch1) ) {
		//convex hull is a polyhedron
		//iterate over bounding planes of convex hull and check whether the point is on the positive side of a plane
        
		//compute plane equations of polyhedron's facets and iterate over all planes
		compute_facet_planes(poly1);
		for(Polyhedron_3::Plane_const_iterator plane = poly1.planes_begin(); plane != poly1.planes_end(); ++plane) {
            if (plane->has_on_positive_side(p))
                return false;
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


Point_3 CHAlgorithms::transform(const Vector3d& point) {
	Point_3 point_3(point(0), point(1), point(2));
	return point_3;
}

Vector3d CHAlgorithms::transform(const Point_3& point_3) {
	Vector3d point;
	point(0) = CGAL::to_double(point_3.x());
	point(1) = CGAL::to_double(point_3.y());
	point(2) = CGAL::to_double(point_3.z());
	return point;
}

std::vector<Vector3d> CHAlgorithms::transform(const std::vector<Point_3>& points) {
    std::vector<Vector3d> points_copy;
    BOOST_FOREACH(const Point_3& point, points) {
        points_copy.push_back(transform(point));
    }
    return points_copy;
}

std::vector<Point_3> CHAlgorithms::transform(const std::vector<Vector3d>& points) {
    std::vector<Point_3> points_copy;
    BOOST_FOREACH(const Vector3d& point, points) {
        points_copy.push_back(transform(point));
    }
    return points_copy;
}

Vector3d CHAlgorithms::random_point_in_ch(const std::vector<Vector3d>& points, unsigned int seed) {
	Vector3d rnd_point;
	Polyhedron_3 poly;
	Segment_3 seg;
	Point_3 point;

	//compute convex hull
    CGAL::Object ch;
    std::vector<Point_3> cgal_points = transform(points);
    CGAL::convex_hull_3(cgal_points.begin(), cgal_points.end(), ch);

	//determine type of convex hull
	if (CGAL::assign(poly, ch)) {

		Polyhedron_3 min_box;

		//determine minimum and maximum x, y and z coordinates
        Polyhedron_3::Point_const_iterator u = poly.points_begin();
        Kernel::FT min_x = u->x();
		Kernel::FT max_x = u->x();
		Kernel::FT min_y = u->y();
		Kernel::FT max_y = u->y();
		Kernel::FT min_z = u->z();
		Kernel::FT max_z = u->z();
		while(++u != poly.points_end()) {
			if(u->x() < min_x)
				min_x = u->x();
			if(u->x() > max_x)
				max_x = u->x();
			if(u->y() < min_y)
				min_y = u->y();
			if(u->y() > max_y)
				max_y = u->y();
			if(u->z() < min_z)
				min_z = u->z();
			if(u->z() > max_z)
				max_z = u->z();

			++u;
		}
		boost::shared_ptr<DistributionGenerator> dist_gen_x;
		boost::shared_ptr<DistributionGenerator> dist_gen_y;
		boost::shared_ptr<DistributionGenerator> dist_gen_z;
		dist_gen_x.reset(new DistributionGenerator(seed));
		dist_gen_y.reset(new DistributionGenerator(seed+1));
		dist_gen_z.reset(new DistributionGenerator(seed+2));
        
		dist_gen_x->init_uniform_real(CGAL::to_double(min_x), CGAL::to_double(max_x));
		dist_gen_y->init_uniform_real(CGAL::to_double(min_y), CGAL::to_double(max_y));
		dist_gen_z->init_uniform_real(CGAL::to_double(min_z), CGAL::to_double(max_z));

        Point_3 rnd_point_in_min_box;
		do {
			//choose random point in minimum surrounding box
			rnd_point_in_min_box = Point_3(dist_gen_x->get_value_uniform_real(),
			                               dist_gen_y->get_value_uniform_real(),
			                               dist_gen_z->get_value_uniform_real());

		} while(!point_contained_in_convex_hull_of_points(rnd_point_in_min_box, cgal_points));

		rnd_point = transform(rnd_point_in_min_box);

	} else if (CGAL::assign(seg, ch)) {

		//TODO(martinah) implement
        throw UnsupportedOperationException("Random point of a degenerated convex hull (segment) not yet supported.");
	} else if (CGAL::assign(point, ch)) {
		rnd_point = transform(point);

	} else {
		throw UnsupportedOperationException("Type of convex hulls couldn't be determined.");
	}

	return rnd_point;
}

/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * points_sepplane.cc
 *
 *  Created on: 25.07.2009
 *      Author: peter
 */

#include <boost/foreach.hpp>

#include "points_sepplane.h"
#include "ch_algorithms.h"
#include "../Utilities/unsupported_operation_exception.h"


// contains non-public helper methods
namespace {
    // mimics std::count template, which seems to have problems with the ublas vector templates
    int count(std::vector<Vector3d>::const_iterator first, std::vector<Vector3d>::const_iterator last, const Vector3d& value) {
        int i=0;
        for (;first != last; ++first)
            if (*first == value) ++i;
        return i;
    }
    
    /**
     * Helper method for separate_point_from_points(). Returns the plane with maximum distance to v that separates v
     * from the given point. Method assumes v to be unequal to w.
     * The normal vector of the returned plane will be pointing towards v.
     */
    const Plane_3 separate_point_from_point(const Point_3& v, const Point_3& w) {
        return Plane_3(w, v - w);
    }
    
    
    /**
     * Helper method for separate_point_from_points(). Returns the plane with maximum distance to v that separates v
     * from the given line. Method assumes v not to lie on the line.
     * The normal vector of the returned plane will be pointing towards v.
     */    
    const Plane_3 separate_point_from_line(const Point_3& v, const Line_3& line) {
        const Point_3 projected_v = line.projection(v);
        return Plane_3(projected_v, v - projected_v);
    }
    
    
    /**
     * Helper method for separate_point_from_points(). Returns the plane with maximum distance to v that separates v
     * from the given segment. Method assumes v not to lie on the segment.
     * The normal vector of the returned plane will be pointing towards v.
     */
    const Plane_3 separate_point_from_segment(const Point_3& v, const Segment_3& segment) {
        const Vector_3 segment_vector = segment.to_vector();
        if (Plane_3(segment.target(), segment_vector).has_on_positive_side(v))
            return separate_point_from_point(v, segment.target());
        else if (Plane_3(segment.source(), -segment_vector).has_on_positive_side(v))
            return separate_point_from_point(v, segment.source());
        else
            return separate_point_from_line(v, segment.supporting_line());
    }
    
    
    /**
     * Helper method for separate_point_from_points(). Returns the plane with maximum distance to v that separates v
     * from the given (convex) polyhedron. Method assumes v not to be in the polyhedron.
     * The normal vector of the returned plane will be pointing towards v.
     */
    const Plane_3 separate_point_from_polyhedron(const Point_3& v, const Polyhedron_3& polyhedron) {
        // the convex hull may be degenerated to a plane --> check for this case
        if (!polyhedron.is_closed()) {
            const Plane_3& plane = *(polyhedron.planes_begin());
            std::vector<Point_3> points;
            Polyhedron_3::Vertex_const_iterator vertex_it;
            Vector_3 normal_vector = plane.orthogonal_vector();
            
            // use a trick: blow degenerated polyhedron up to a non-degenerated polyhedron
            if (polyhedron.planes_begin()->has_on_positive_side(v))
                normal_vector = -normal_vector;
            for (vertex_it=polyhedron.vertices_begin(); vertex_it!=polyhedron.vertices_end(); ++vertex_it)
                points.push_back(vertex_it->point());
            for (vertex_it=polyhedron.vertices_begin(); vertex_it!=polyhedron.vertices_end(); ++vertex_it)
                points.push_back(vertex_it->point() + normal_vector);
            
            // now compute separation plane for non-degenerated polyhedron
            Polyhedron_3 nondegenerated_poly;
            CGAL::convex_hull_3(points.begin(), points.end(), nondegenerated_poly);
            CHAlgorithms::compute_facet_planes(nondegenerated_poly);
            return separate_point_from_polyhedron(v, nondegenerated_poly);
        }
        
        // first, we check whether v is directly above one of the facets (--> use facet's plane as separation plane)
        Polyhedron_3::Facet_const_iterator facet_it;
        for (facet_it = polyhedron.facets_begin(); facet_it != polyhedron.facets_end(); ++facet_it) {
            if (!facet_it->plane().has_on_positive_side(v))
                continue;
            
            // construct planes that encapsulate the (infinite) cylinder defined by the facet
            std::list<Plane_3> bounding_planes;
            Polyhedron_3::Halfedge_const_handle facet_edge = facet_it->halfedge();
            for (std::size_t i=0; i<facet_it->facet_degree(); ++i) {
                const Vector_3 edge_vector = facet_edge->vertex()->point() - facet_edge->prev()->vertex()->point();
                const Vector_3 normal = CGAL::cross_product(facet_it->plane().orthogonal_vector(), edge_vector);
                bounding_planes.push_back(Plane_3(facet_edge->vertex()->point(), normal));
                facet_edge = facet_edge->next();
            }
            
            // if we're inside or on the cylinder (and above the facet), the facet's plane will do as separation plane
            bool valid = true;
            BOOST_FOREACH(const Plane_3& bounding_plane, bounding_planes) {
                if (bounding_plane.has_on_negative_side(v)) {
                    valid = false;
                    break;
                }
            }
            if (valid)
                return Plane_3(facet_it->plane());
        }
        
        // if we're not straight above a facet, check if we're "straight above" an edge
        Polyhedron_3::Edge_const_iterator edge_it;
        for (edge_it = polyhedron.edges_begin(); edge_it != polyhedron.edges_end(); ++edge_it) {
            std::list<Plane_3> bounding_planes;
            const Vector_3 edge_vector = edge_it->vertex()->point() - edge_it->prev()->vertex()->point();
            
            // bounding planes towards incident facets
            const Vector_3 normal1 = CGAL::cross_product( edge_vector, edge_it->facet()->plane().orthogonal_vector());
            const Vector_3 normal2 = CGAL::cross_product(-edge_vector, edge_it->opposite()->facet()->plane().orthogonal_vector());
            bounding_planes.push_back(Plane_3(edge_it->vertex()->point(), normal1));
            bounding_planes.push_back(Plane_3(edge_it->vertex()->point(), normal2));
            
            // bounding planes towards incident vertices
            bounding_planes.push_back(Plane_3(edge_it->vertex()->point(),             -edge_vector));
            bounding_planes.push_back(Plane_3(edge_it->opposite()->vertex()->point(),  edge_vector));
            
            // if v lies inside the bounding planes, the separation plane is given by separating v from the edge
            bool valid = true;
            BOOST_FOREACH(const Plane_3& bounding_plane, bounding_planes) {
                if (bounding_plane.has_on_negative_side(v)) {
                    valid = false;
                    break;
                }
            }
            if (valid)
                return separate_point_from_line(v, Line_3(edge_it->vertex()->point(), edge_vector));
        }
        
        // finally, we have to find a vertex that defines the separation plane
        Polyhedron_3::Vertex_const_iterator vertex_it;
        for (vertex_it = polyhedron.vertices_begin(); vertex_it != polyhedron.vertices_end(); ++vertex_it) {
            // compute bounding planes of incident edges
            std::list<Plane_3> bounding_planes;
            Polyhedron_3::Halfedge_const_handle vertex_edge = vertex_it->halfedge();
            for (std::size_t i=0; i<vertex_it->vertex_degree(); ++i) {
                const Vector_3 edge_vector = vertex_edge->vertex()->point() - vertex_edge->prev()->vertex()->point();
                bounding_planes.push_back(Plane_3(vertex_it->point(), edge_vector));
                vertex_edge = vertex_edge->next_on_vertex();
            }
            
            // if v lies inside the bounding planes, the separation plane is given by separating v from the vertex
            bool valid = true;
            BOOST_FOREACH(const Plane_3& bounding_plane, bounding_planes) {
                if (bounding_plane.has_on_negative_side(v)) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                return separate_point_from_point(v, vertex_it->point());
            }
        }
        
        // we should never reach this
        throw std::runtime_error("Unexpected error: could not find a separation plane.");
    }
    
    
    /**
     * Helper method for separate_point_from_points(). Returns the plane with maximum distance to v that separates v
     * from the given triangle. Method assumes v not to lie on the triangle.
     * The normal vector of the returned plane will be pointing towards v.
     */
    const Plane_3 separate_point_from_triangle(const Point_3& v, const Triangle_3& triangle) {
        // use a trick: blow triangle up to a non-degenerated polyhedron
        Vector_3 normal_vector = triangle.supporting_plane().orthogonal_vector();
        
        std::vector<Point_3> points;
        if (triangle.supporting_plane().has_on_positive_side(v))
            normal_vector = -normal_vector;
        points.push_back(triangle[0]);
        points.push_back(triangle[1]);
        points.push_back(triangle[2]);
        points.push_back(triangle[0] + normal_vector);
        points.push_back(triangle[1] + normal_vector);
        points.push_back(triangle[2] + normal_vector);
        
        // now compute separation plane for non-degenerated polyhedron
        Polyhedron_3 nondegenerated_poly;
        CGAL::convex_hull_3(points.begin(), points.end(), nondegenerated_poly);
        CHAlgorithms::compute_facet_planes(nondegenerated_poly);
        return separate_point_from_polyhedron(v, nondegenerated_poly);
        
    }
}


const Vector3d separate_point_from_points(const Vector3d& v, const std::vector<Vector3d>& w, double epsilon) {
    // filter duplicates of v from w and convert input to cgal objects
    // filter duplicates in w itself (workaround: CGAL has problems computing convex hull of several equal points)
    Point_3 cgal_v = CHAlgorithms::transform(v);
    std::vector<Point_3> filtered_cgal_w;
    std::vector<Vector3d>::const_iterator w_it;
    for (w_it=w.begin(); w_it != w.end(); ++w_it) {
        if (boost::numeric::ublas::norm_2(*w_it - v) > epsilon && count(w_it, w.end(), *w_it) == 1)
            filtered_cgal_w.push_back(CHAlgorithms::transform(*w_it));
    }
    
    // check whether there are any w's left; if not, v was on same spot as w's --> return null vector
    if (filtered_cgal_w.empty())
        return boost::numeric::ublas::zero_vector<double>(3);
    
    // check whether v is contained in the convex hull of the w's
    if (CHAlgorithms::point_contained_in_convex_hull_of_points(cgal_v, filtered_cgal_w))
        return boost::numeric::ublas::zero_vector<double>(3);
    
    // compute convex hul of the w's
    CGAL::Object convex_hull;
    CGAL::convex_hull_3(filtered_cgal_w.begin(), filtered_cgal_w.end(), convex_hull);
    
    // check for degenerated cases
    Plane_3 sep_plane;
    if (CGAL::object_cast<Polyhedron_3>(&convex_hull)) {
        // we need a writable copy of the polyhedron to compute its plane equations
        Polyhedron_3 polyhedron;
        CGAL::assign(polyhedron, convex_hull);
        CHAlgorithms::compute_facet_planes(polyhedron);
        
        sep_plane = separate_point_from_polyhedron(cgal_v, polyhedron);
    } else if (const Triangle_3* triangle = CGAL::object_cast<Triangle_3>(&convex_hull)) {
        sep_plane = separate_point_from_triangle(cgal_v, *triangle);
    } else if (const Segment_3* segment = CGAL::object_cast<Segment_3>(&convex_hull)) {
        sep_plane = separate_point_from_segment(cgal_v, *segment);
	} else if (CGAL::object_cast<Point_3>(&convex_hull)) {
        sep_plane = separate_point_from_point(cgal_v, filtered_cgal_w[0]);
	} else {
		throw std::runtime_error("Type of convex hull couldn't be determined (is neither plane, segment or point).");
	}
    
    // compute return vector
    const Point_3 cgal_ret_vector = CGAL::ORIGIN + (sep_plane.projection(cgal_v) - cgal_v);
    return CHAlgorithms::transform(cgal_ret_vector);
}

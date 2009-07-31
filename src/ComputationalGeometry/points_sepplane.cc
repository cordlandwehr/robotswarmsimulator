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


// CGAL typedefs
typedef Polyhedron_3::Facet    Facet;
typedef Polyhedron_3::Halfedge Halfedge;
typedef Polyhedron_3::Vertex   Vertex;
typedef Kernel::Vector_3       Vector_3;
typedef Kernel::Line_3         Line_3;
typedef Kernel::Plane_3        Plane_3;


// contains non-public helper methods
namespace {
    /**
     * Given a facet, this method computes the planes corresponding to any of the polyhedron's facets.
     */
    void compute_facet_planes(Polyhedron_3& polyhedron) {
        Polyhedron_3::Facet_iterator facet_it;
        for (facet_it = polyhedron.facets_begin(); facet_it != polyhedron.facets_end(); ++facet_it) {
            Polyhedron_3::Facet::Halfedge_handle edge = facet_it->halfedge();
            facet_it->plane() = Plane_3(edge->vertex()->point(),
                                        edge->next()->vertex()->point(),
                                        edge->next()->next()->vertex()->point());
        }
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
}


const Vector3d separate_point_from_points(const Vector3d& v, const std::vector<Vector3d>& w) {
    // filter duplicates of v from w
    std::vector<Vector3d> filtered_w;
    BOOST_FOREACH(const Vector3d& w_elem, w) {
        if (w_elem != v)
            filtered_w.push_back(w_elem);
    }
    
    // check whether there are any w's left; if not, v was on same spot as w's --> return null pointer
    if (filtered_w.empty())
        return boost::numeric::ublas::zero_vector<double>(3);
    
    // check whether v is contained in the convex hull of the w's
    if (CHAlgorithms::point_contained_in_convex_hull_of_points(v, filtered_w))
        return boost::numeric::ublas::zero_vector<double>(3);
    
    // compute convex hul of the w's and check for degenerated cases
    Polyhedron_3 polyhedron;
    Segment_3 segment;
    Point_3 point;
    Plane_3 sep_plane(0, 0, 0, 0);
    CGAL::Object convex_hull = CHAlgorithms::compute_convex_hull_3d(filtered_w);
    const Point_3 cgal_v = CHAlgorithms::vector3d_to_point_3(v);
    if (CGAL::assign(polyhedron, convex_hull)) {
        compute_facet_planes(polyhedron);
        sep_plane = separate_point_from_polyhedron(cgal_v, polyhedron);
    } else if (CGAL::assign(segment, convex_hull)) {
        sep_plane = separate_point_from_segment(cgal_v, segment);
	} else if (CGAL::assign(point, convex_hull)) {
        sep_plane = separate_point_from_point(cgal_v, CHAlgorithms::vector3d_to_point_3(filtered_w[0]));
	} else {
		throw std::runtime_error("Type of convex hull couldn't be determined (is neither plane, segment or point).");
	}
    
    // compute return vector
    const Point_3 cgal_ret_vector = CGAL::ORIGIN + (sep_plane.projection(cgal_v) - cgal_v);
    return CHAlgorithms::point_3_to_vector3d(cgal_ret_vector);
}

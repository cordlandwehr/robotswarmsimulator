/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 * ch_algorithms.h
 *
 *  Created on: 27.03.2009
 *      Author: martinah
 *
 */

#ifndef CH_ALGORITHMS_H_
#define CH_ALGORITHMS_H_

#include <vector>

#include <boost/smart_ptr.hpp>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/convex_hull_3.h>

#include "../Utilities/vector_arithmetics.h"


// CGAL typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel   Kernel;
typedef CGAL::Convex_hull_traits_3<Kernel>                  CHTraits;
typedef CHTraits::Polyhedron_3                              Polyhedron_3;
typedef CHTraits::Triangle_3                                Triangle_3;
typedef Kernel::Segment_3                                   Segment_3;
typedef Kernel::Point_3                                     Point_3;
typedef Kernel::Vector_3                                    Vector_3;
typedef Polyhedron_3::Facet                                 Facet;
typedef Polyhedron_3::Halfedge                              Halfedge;
typedef Polyhedron_3::Vertex                                Vertex;
typedef Kernel::Line_3                                      Line_3;
typedef Kernel::Plane_3                                     Plane_3;

/**
 * This class provides some useful algorithms working with a convex hull.
 */
class CHAlgorithms {

	friend class point_contained_in_ch_polyhedron_test;
	friend class point_contained_in_ch_segment_test;
	friend class point_contained_in_ch_point_test;
	friend class rnd_poin_in_ch;

public:
    
    /**
	 * This method computes the COG of the convex hull of the given set of points.
	 * \param points	Set of points of which to compute CH
	 * \return			COG of the convex hull of the given set of points.
	 */
	static Vector3d compute_cog_of_ch_of_points(const std::vector<Vector3d>& points);
    
    /**
	 * This method checks whether the given point is contained in the given convex hull of the given points.
	 * \param point		Point to check.
	 * \param points	Points of whicht to compute convex hull.
	 * \return			true, if the given point is contained in or on the boundary of the convex hull
	 * 					of the given points, false otherwise.
	 */
	static bool point_contained_in_convex_hull_of_points(const Vector3d& point, const std::vector<Vector3d>& points);
    
    /**
	 * This method returns a point chosen uniformly at random from the convex hull of the given points
	 * \param points	Points whose convex hull to compute from which to choose random point.
	 * \param seed		Seed
	 * \return 			Point chosen chosen uniformly at random from the convex hull of the given points.
	 */
	static Vector3d random_point_in_ch(const std::vector<Vector3d>& points, unsigned int seed);
    
    
    
    /**
	 * This method checks whether the given point is contained in the given convex hull of the given points.
	 * \param point		Point to check.
	 * \param points	Points of whicht to compute convex hull.
	 * \return			true, if the given point is contained in or on the boundary of the convex hull
	 * 					of the given points, false otherwise.
	 */
	static bool point_contained_in_convex_hull_of_points(const Point_3& point, const std::vector<Point_3>& points);
    
    /**
     * Given a polyhedron, this method computes the planes corresponding to any of the polyhedron's facets. The planes
     * are saved within the 'plane' attribute of the facets.
     */
    static void compute_facet_planes(Polyhedron_3& polyhedron);
    
	/**
	 * This method computes the COG of the given polyhedron.
	 * \param poly		Polyhedron of which to compute COG
	 * \return			COG of the given polyhedron
	 */
	static Point_3 compute_cog_of_polyhedron(const Polyhedron_3& poly);
    
	/**
	 * This method computes the COG of the given triangle.
	 * \param triangle Triangle of which to compute COG
	 * \return         COG of the given triangle
	 */
	static Point_3 compute_cog_of_triangle(const Triangle_3& triangle);
    
	/**
	 * This method computes the COG of the given segment.
	 * I.e. this mehtod only computes the center of the endpoints of the given segment.
	 * \param seg			Segment which COG to compute.
	 * \return				Center of given segment segment.
	 */
	static Point_3 compute_cog_of_segment(const Segment_3& seg);

	/**
	 * This method transforms the given Vector3d to a Point_3.
	 * \param point		Vector3d to transform to a Point_3
	 * \return 			Point_3 of the given Vector3d
	 */
	static Point_3 transform(const Vector3d& point);

	/**
	 * This method transforms the given Point_3 to a Vector3d.
	 * \param point_3   Point_3 to transform to a Vector3d
	 * \return 			Vector3d of the given Point_3
	 */
	static Vector3d transform(const Point_3& point_3);
    
    /**
	 * This method transforms the given std::vector of Point_3 to a std::vector of Vector3d.
	 */
	static std::vector<Vector3d> transform(const std::vector<Point_3>& points);
    
    /**
	 * This method transforms the given std::vector of Vector3d to a std::vector of Point_3.
	 */
	static std::vector<Point_3> transform(const std::vector<Vector3d>& points);
    
	/**
	 * This method prints the vertices of the given convex hull.
	 * \param ch	Convex hull which vertices to print.
	 */
	static void print_vertices_of_ch(const CGAL::Object& ch);

	/**
	 * This method prints the endpoints of the given segment.
	 * \param seg	Segment whose endpoints to print.
	 */
	static void print_vertices_of_segment(const Segment_3& seg);

	/**
	 * This method prints the vertices of the given polyhedron.
	 * \param poly	Polyhedron whose vertices to print.
	 */
	static void print_vertices_of_polyhedron(const Polyhedron_3& poly);
};

#endif /* CH_ALGORITHMS_H_ */

/*
 * diameter.cc
 *
 *  Created on: 27.08.2009
 *      Author: dwarner
 */

#include "diameter.h"
#include "ch_algorithms.h"
#include "../Utilities/vector_arithmetics.h"
#include <limits>

// contains non-public helper methods
namespace {
	// returns the signed area of the triangle with vertices (x, y, z)
	const double area(const Vector3d& x, const Vector3d& y, const Vector3d& z) {
    	double a;

    	const Vector3d u = y - x;
    	const Vector3d v = z - x;

    	Vector3d cross;
    	cross(0) = u(1) * v(2) - u(2) * v(1);
    	cross(1) = u(2) * v(0) - u(0) * v(2);
    	cross(2) = u(0) * v(1) - u(1) * v(0);

    	double l = vector3d_get_length(cross, 2);

    	// TODO calculate sign

    	return l;
    }
}

const double calculate_diameter(const std::vector<Vector3d>& w) {
	return calculate_diameter(w, true);
}

const double calculate_diameter(const std::vector<Vector3d>& w, const bool efficient) {
	const int w_size = w.size();

	if (w_size == 0 || w_size == 1)
		return 0;

	double dist = std::numeric_limits<double>::max();
	double t_dist;

	if (efficient) {
		std::vector<Point_3> ch_p;
		std::vector<Vector3d> ch_points;

		CGAL::Object ch; // define object to hold convex hull
		std::vector<Point_3> cgal_w = CHAlgorithms::transform(w);
		CGAL::convex_hull_3(cgal_w.begin(), cgal_w.end(), ch); // compute convex hull

		// determine what kind of object it is
		if (CGAL::object_cast<Segment_3>(&ch))
		{
			// convex hull is a segment
			const Segment_3* seg = CGAL::object_cast<Segment_3>(&ch);
			ch_p.push_back(seg->source());
			ch_p.push_back(seg->target());
		}
		else if (CGAL::object_cast<Polyhedron_3>(&ch)) {
			// convex hull is a polyhedron
			const Polyhedron_3* poly = CGAL::object_cast<Polyhedron_3>(&ch);
			int num = 0;
			for (Polyhedron_3::Vertex_const_iterator v = poly->vertices_begin(); v != poly->vertices_end(); ++v) {
			   	ch_p.push_back(v->point());
			}
		}
		else if (CGAL::object_cast<Triangle_3>(&ch)) {
			// convex hull is a triangle
			const Triangle_3* triangle = CGAL::object_cast<Triangle_3>(&ch);
			ch_p.push_back(triangle->vertex(0));
			ch_p.push_back(triangle->vertex(1));
			ch_p.push_back(triangle->vertex(2));
		}
		else if (CGAL::object_cast<Point_3>(&ch)) {
			// convex hull is a point
			const Point_3* point = CGAL::object_cast<Point_3>(&ch);
			ch_p.push_back(*point);
		}
		else {
			std::cout << "convex hull error!" << std::endl;
		}

	    ch_points = CHAlgorithms::transform(ch_p);

	    // TODO
	    for (int i = 0; i < ch_points.size(); i++)
	    {
	    	std::cout << ch_points.at(i) << " @ ";
	    }

		std::vector<Vector3d> pairs;
		const Vector3d first = ch_points.at(0);
		const Vector3d last = ch_points.at(ch_points.size() - 1);

		int k = 0; // TODO k=1?
		while (k < ch_points.size() - 1 && area(last, first, ch_points.at(k + 1)) > area(last, first, ch_points.at(k))) {
			k++;
		}

	    std::cout << "STEP 2";

		int i = 0;
		int j = k;

		while (i <= k && j < ch_points.size()) {
			std::cout << "outer loop" << "i: " << i << " j: " << j << " k: " << k << "  ";
			pairs.push_back(ch_points.at(i));
			pairs.push_back(ch_points.at(j));
			while (area(ch_points.at(i), ch_points.at(i + 1), ch_points.at(j + 1)) > area(ch_points.at(i), ch_points.at(i + 1), ch_points.at(j)) && j < ch_points.size() - 1) {
				std::cout << "inner loop " << "i: " << i << " j: " << j << " k: " << k << "  ";
				pairs.push_back(ch_points.at(i));
				pairs.push_back(ch_points.at(j));
				j++;
			}
			i++;
		}

	    std::cout << "STEP 3";

		for (int i = 0; i < pairs.size() - 2; i += 2)
		{
			t_dist = vector3d_distance(pairs.at(i), pairs.at(i + 1));
			if (t_dist < dist)
				dist = t_dist;
		}

	    std::cout << "STEP 4";

		return dist;
	} else
	{
		Vector3d w_u = w.at(0);
		Vector3d w_v = w.at(1);

		for (int i = 0; i < w_size - 1; i++)
		{
			w_u = w.at(i);
			for (int j = i + 1; j < w_size; j++)
			{
				w_v = w.at(j);
				t_dist = vector3d_distance(w_u, w_v);
				if (t_dist < dist)
					dist = t_dist;
			}
		}

		return dist;
	}
}

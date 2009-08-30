/*
 * diameter.cc
 *
 *  Created on: 27.08.2009
 *      Author: dwarner
 */

#include "diameter.h"
#include "gdiam.h"
#include "../Utilities/vector_arithmetics.h"

std::vector<Vector3d> DiameterAlgorithms::compute_diametrical_pair(
		const std::vector<Vector3d>& points, const double epsilon) {
	const int n = points.size();

	if (n == 0 || n == 1) {
		Vector3d vector;
		std::vector<Vector3d> result;

		if (n == 0) {
			vector(0) = 0;
			vector(1) = 0;
			vector(2) = 0;

			result.push_back(vector);
			result.push_back(vector);
		} else {
			vector(0) = points.at(0)(0);
			vector(1) = points.at(0)(1);
			vector(2) = points.at(0)(2);

			result.push_back(vector);
			result.push_back(vector);
		}

		return result;
	}

	gdiam_real * pts;

	pts = (gdiam_point) malloc(sizeof(gdiam_point_t) * n);
	assert(pts != NULL);

	for (int i = 0; i < n; i++) {
		Vector3d v = points.at(i);
		pts[i * 3 + 0] = v(0);
		pts[i * 3 + 1] = v(1);
		pts[i * 3 + 2] = v(2);
	}

	gdiam_real eps = (gdiam_real) epsilon;

	GPointPair pair = gdiam_approx_diam_pair(pts, n, eps);

	std::vector<Vector3d> return_pair;

	Vector3d p, q;
	p(0) = pair.p[0];
	p(1) = pair.p[1];
	p(2) = pair.p[2];
	q(0) = pair.q[0];
	q(1) = pair.q[1];
	q(2) = pair.q[2];
	return_pair.push_back(p);
	return_pair.push_back(q);

	free(pts);

	return return_pair;
}

double DiameterAlgorithms::compute_diameter(
		const std::vector<Vector3d>& points, const double epsilon) {
	std::vector<Vector3d> pair = compute_diametrical_pair(points, epsilon);
	return vector3d_distance(pair.at(0), pair.at(1));
}

double DiameterAlgorithms::compute_diameter_naive(
		const std::vector<Vector3d>& points) {
	const int n = points.size();

	if (n == 0 || n == 1)
		return 0;

	double dist = 0;
	double t_dist;

	Vector3d u;
	Vector3d v;

	for (int i = 0; i < n - 1; i++) {
		u = points.at(i);
		for (int j = i + 1; j < n; j++) {
			v = points.at(j);
			t_dist = vector3d_distance(u, v);
			if (t_dist > dist)
				dist = t_dist;
		}
	}

	return dist;
}

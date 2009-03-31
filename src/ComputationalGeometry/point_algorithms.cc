/*
 * tp_algoritms.cc
 *
 *  Created on: 21.03.2009
 *      Author: sven
 */

#include "point_algorithms.h"
#include "miniball.h"
#include "miniball.cc"
#include "miniball_b.h"
#include "miniball_b.cc"

Vector3d PointAlgorithms::compute_COG(const std::vector<Vector3d>& positions) {
	Vector3d cog;
	cog(0) = 0;
	cog(1) = 0;
	cog(2) = 0;

	std::vector<Vector3d>::const_iterator iter;

	for (iter = positions.begin(); iter != positions.end(); iter++) {
		cog(0) += (*iter)(0);
		cog(1) += (*iter)(1);
		cog(2) += (*iter)(2);
	}

	cog /= positions.size();

	return cog;
}

Vector3d PointAlgorithms::compute_CMinBall(const std::vector<Vector3d>& positions) {
	Miniball<3> miniball;
	miniball.check_in(positions);
	miniball.build();
	Point<3> point = miniball.center();

	// build a vector3d to return
	Vector3d result;
	result(0) = point[0];
	result(1) = point[1];
	result(2) = point[2];

	return result;
}

Vector3d PointAlgorithms::compute_CMinRect(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_MaxLine(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_CCH(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_RCH(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_RMinRect(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_ACH(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_MidFar(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_MED(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_FW(const std::vector<Vector3d>& positions) {
	return Vector3d();
}

Vector3d PointAlgorithms::compute_kMin(const std::vector<Vector3d>& positions, int k) {
	return Vector3d();
}

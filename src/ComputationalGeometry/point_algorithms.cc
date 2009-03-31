/*
 * tp_algoritms.cc
 *
 *  Created on: 21.03.2009
 *      Author: sven
 */

#include "point_algorithms.h"

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
	// cog(0) = cog(0)/point_list.size();
	// cog(1) = cog(1)/point_list.size();
	// cog(2) = cog(2)/point_list.size();
	cog /= positions.size();

	return cog;
}

Vector3d PointAlgorithms::compute_CMinBall(const std::vector<Vector3d>& positions) {
	return Vector3d();
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

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
#include "../Utilities/console_output.h"

#include <boost/foreach.hpp>

Vector3d PointAlgorithms::compute_COG(const std::vector<Vector3d>& positions) {
	if(positions.size() == 0) {
		// do some error handling.
		ConsoleOutput::out_error("Computing COG with positions.size == 0", ConsoleOutput::ComputationalGeometry);
	}

	Vector3d cog;
	cog(0) = 0;
	cog(1) = 0;
	cog(2) = 0;

	std::vector<Vector3d>::const_iterator iter;

	BOOST_FOREACH(Vector3d position, positions) {
		cog += position;
	}

	cog /= positions.size();

	return cog;
}

Vector3d PointAlgorithms::compute_CMinBall(const std::vector<Vector3d>& positions) {
	if(positions.size() == 0) {
		// do some error handling
		ConsoleOutput::out_error("Computing CMinBall with positions.size == 0", ConsoleOutput::ComputationalGeometry);
	}

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

Vector3d PointAlgorithms::compute_CMinBox(const std::vector<Vector3d>& positions) {
	if(positions.size() == 0) {
		// do some error handling
		ConsoleOutput::out_error("Computing CMinBox with positions.size == 0", ConsoleOutput::ComputationalGeometry);
	}
	Vector3d first = positions[0];

	BOOST_FOREACH(Vector3d position, positions) {

	}

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

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
#include "../Utilities/vector_arithmetics.h"

#include <boost/foreach.hpp>

Vector3d PointAlgorithms::compute_COG(const std::vector<Vector3d>& positions) {
	if(positions.size() == 0) {
		// do some error handling.
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error) << "Computing COG with positions.size == 0";
		return Vector3d();
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
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error) << "Computing CMinBall with positions.size == 0";
		return Vector3d();
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
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error) << "Computing CMinBox with positions.size == 0";
		return Vector3d();
	}
	Vector3d first = positions.at(0);
	double min_x = first(kXCoord);
	double max_x = first(kXCoord);
	double min_y = first(kYCoord);
	double max_y = first(kYCoord);
	double min_z = first(kZCoord);
	double max_z = first(kZCoord);

	BOOST_FOREACH(Vector3d position, positions) {
		if(position(kXCoord) < min_x) {
			min_x = position(kXCoord);
		} else if(position(kXCoord) > max_x) {
			max_x = position(kXCoord);
		}

		if(position(kYCoord) < min_y) {
			min_y = position(kYCoord);
		} else if(position(kYCoord) > max_y) {
			max_y = position(kYCoord);
		}

		if(position(kZCoord) < min_z) {
			min_z = position(kZCoord);
		} else if(position(kZCoord) > max_z) {
			max_z = position(kZCoord);
		}
	}

	Vector3d CMinBox;
	CMinBox(kXCoord) = (min_x + max_x) / 2;
	CMinBox(kYCoord) = (min_y + max_y) / 2;
	CMinBox(kZCoord) = (min_z + max_z) / 2;

	return CMinBox;
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

Vector3d PointAlgorithms::compute_MidFar(const Vector3d & own_position, const std::vector<Vector3d>& positions) {
	if(positions.size() == 0) {
		// do some error handling.
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error) << "Computing MidFar with positions.size == 0";
		return Vector3d();
	}

	double longest_distance = vector3d_distance(own_position, positions.at(0));
	Vector3d longest_distance_vec = positions.at(0);

	std::vector<Vector3d>::const_iterator iter;

	BOOST_FOREACH(Vector3d position, positions) {
		double cur_distance = vector3d_distance(own_position, position);
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::debug) << "cur distance: " << cur_distance;
		if(cur_distance > longest_distance) {
			longest_distance = cur_distance;
			longest_distance_vec = position;
		}
	}

	Vector3d target_point = own_position + 0.5 * (longest_distance_vec - own_position);

	return target_point;
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

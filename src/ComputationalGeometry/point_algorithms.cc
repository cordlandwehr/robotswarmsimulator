/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
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

namespace {
bool compareX (const Vector3d& i,const Vector3d& j) { return (i[kXCoord] < j[kXCoord]); }
bool compareY (const Vector3d& i,const Vector3d& j) { return (i[kYCoord] < j[kYCoord]); }
bool compareZ (const Vector3d& i,const Vector3d& j) { return (i[kZCoord] < j[kZCoord]); }
}

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

Vector3d PointAlgorithms::compute_MaxLine(int coord, const std::vector<Vector3d>& positions) {
	// tp is extreme position with regard to coord
	if(coord < 0 || coord > 2 || positions.size() == 0) {
		// do some error handling
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error) << "Computing MaxLine with invalid parameters";
		return Vector3d();
	}

	double extreme_distance = positions.at(0)[coord];
	Vector3d extreme_vec = positions.at(0);

	BOOST_FOREACH(Vector3d position, positions) {
		double cur_extreme_distance = position[coord];
		if(cur_extreme_distance > extreme_distance) {
			extreme_distance = cur_extreme_distance;
			extreme_vec = position;
		}
	}

	return extreme_vec;
}

Vector3d PointAlgorithms::compute_CCH(const std::vector<Vector3d>& positions) {
	// TODO(craupach) needs CGAL
	return Vector3d();
}

Vector3d PointAlgorithms::compute_RCH(const std::vector<Vector3d>& positions) {
	// TODO(craupach) needs CGAL
	return Vector3d();
}

Vector3d PointAlgorithms::compute_RMinRect(const boost::shared_ptr<DistributionGenerator> source_of_randomness, const std::vector<Vector3d>& positions) {
	if(positions.size() == 0) {
		// do some error handling
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error) << "Computing RMinRect with positions.size == 0";
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

	Vector3d target_point;
	source_of_randomness->init_uniform_real(min_x, max_x);
	target_point[kXCoord] = source_of_randomness->get_value_uniform_real();
	source_of_randomness->init_uniform_real(min_y, max_y);
	target_point[kYCoord] = source_of_randomness->get_value_uniform_real();
	source_of_randomness->init_uniform_real(min_z, max_z);
	target_point[kZCoord] = source_of_randomness->get_value_uniform_real();

	return target_point;
}

Vector3d PointAlgorithms::compute_ACH(const std::vector<Vector3d>& positions) {
	// TODO(craupach) This cannot be implemented in  a more arbitrary way than being random (RCH)
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
	std::vector<Vector3d> positions_mutable = positions;

	// get (upper) median for kXCoord, kYCoord, kZCoord by sorting
	// this could use Median of Median algorithms to be more efficient
	// if it causes run-time problems.
	std::sort(positions_mutable.begin(), positions_mutable.end(), &compareX);
	double x = positions_mutable.at(positions_mutable.size() / 2)[kXCoord];
	std::sort(positions_mutable.begin(), positions_mutable.end(), &compareY);
	double y = positions_mutable.at(positions_mutable.size() / 2)[kYCoord];
	std::sort(positions_mutable.begin(), positions_mutable.end(), &compareZ);
	double z = positions_mutable.at(positions_mutable.size() / 2)[kZCoord];

	Vector3d target_point;
	target_point[kXCoord] = x;
	target_point[kYCoord] = y;
	target_point[kZCoord] = z;

	return target_point;
}

Vector3d PointAlgorithms::compute_FW(const std::vector<Vector3d>& positions) {
	// TODO(craupach) complicated to implement ;-)
	return Vector3d();
}

Vector3d PointAlgorithms::compute_kMin(const std::vector<Vector3d>& positions, int k) {
	// TODO(craupach) complicated to implement ;-)
	return Vector3d();
}

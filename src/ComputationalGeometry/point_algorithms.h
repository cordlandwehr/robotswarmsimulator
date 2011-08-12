/*
	This file is part of RobotSwarmSimulator.

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
 * point_algorithms.h
 *
 *  Created on: 21.03.2009
 *      Author: sven
 *
 * \brief
 * This class provides the computation of all the targetpositions
 * analyzed by the convergence-theory-team.
 */

#ifndef TP_ALGORITHMS_H_
#define TP_ALGORITHMS_H_

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"


class PointAlgorithms {
public:

	/**
	 * Explanations see here: https://trac.cs.upb.de/der-schwarm/wiki/Theory/cog/tps
	 *
	 * Note: all random targetpoints will require an seeded distribution generator to be passed
	 * to the algorithm.
	 */

	static Vector3d compute_COG(const std::vector<Vector3d>& positions);
	static Vector3d compute_CMinBall(const std::vector<Vector3d>& positions);
	static Vector3d compute_CMinBox(const std::vector<Vector3d>& positions);
	static Vector3d compute_MaxLine(int coord, const std::vector<Vector3d>& positions);
	static Vector3d compute_CCH(const std::vector<Vector3d>& positions);
	static Vector3d compute_RCH(const std::vector<Vector3d>& positions);
	static Vector3d compute_RMinRect(const boost::shared_ptr<DistributionGenerator> source_of_randomness, const std::vector<Vector3d>& positions);
	static Vector3d compute_ACH(const std::vector<Vector3d>& positions);
	static Vector3d compute_MidFar(const Vector3d & own_position, const std::vector<Vector3d>& positions);
	static Vector3d compute_MED(const std::vector<Vector3d>& positions);
	static Vector3d compute_FW(const std::vector<Vector3d>& positions);
	static Vector3d compute_kMin(const std::vector<Vector3d>& positions, int k);

private:
};

#endif /* TP_ALGORITHMS_H_ */

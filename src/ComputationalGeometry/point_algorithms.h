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

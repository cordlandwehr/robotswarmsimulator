/*
 * tp_algorithms.h
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

#include <boost/smart_ptr.hpp>

#include "vector_arithmetics.h"

class TP_Algorithms {
public:

	/**
	 * Explanations see here: https://trac.cs.upb.de/der-schwarm/wiki/Theory/cog/tps
	 */

	static Vector3d compute_COG(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_CMinBall(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_CMinRect(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_MaxLine(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_CCH(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_RCH(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_RMinRect(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_ACH(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_MidFar(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_MED(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_FW(boost::shared_ptr<std::vector<Vector3d> > positions);
	static Vector3d compute_kMin(boost::shared_ptr<std::vector<Vector3d> > positions, int k);

private:
};

#endif /* TP_ALGORITHMS_H_ */

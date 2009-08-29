/*
 * diameter.h
 *
 *  Created on: 27.08.2009
 *      Author: dwarner
 */

#ifndef DIAMETER_H_
#define DIAMETER_H_

#include "../Utilities/vector_arithmetics.h"

/**
 * \brief This class provides methods for computing the diameter of the convex hull of a given set of points in the three-dimensional euclidean space.
 *
 * This is the same value as the maximum distance realized by two points in the set i.e., the
 * distance between two farthest points in the set.
 *
 * In literature the problem is referred to as farthest-pair-problem or diameter-problem.
 *
 * The straightforward algorithm to solve this problem compares each pair of points
 * and returns the maximum distance realized by two points in the set. This approach leads
 * to a Theta(n^2) time algorithm.
 *
 * There are (asymptotically) more efficient algorithms, as the randomized Pruning-algorithm by Clarkson and Shor [1,2,3],
 * which has expected running time O(n*log n) and the deterministic algorithm by Ramos [4] with an asymptotically optimal running time of O(n*log n).
 * In practice implementing these algorithms is fairly difficult and, moreover, they perform worse than the naive Theta(n^2) algorithm.
 *
 * The present implementation uses code by Sariel Har-Peled based on his paper [5].
 *
 * Literature:
 *
 * [1] CLARKSON, K. L. and P. W. SHOR: Applications of randomized sampling in computational geometry. Discrete Comput. Geom., 4:387–421, 1989.
 * [2] AGARWAL, PANKAJ K. and MICHA SHARIR: Efficient algorithms for geometric optimization. ACM Computing Surveys, 30(4):412–458, 1998.
 * [3] AGARWAL, PANKAJ K. and SANDEEP SEN: Randomized Algorithms for Geometric Optimization Problems, 2000.
 * [4] E. A. RAMOS: Deterministic algorithms for 3-D diameter and some 2-D lower envelopes. Proc. 16th Sympos. Comput. Geom., 2000.
 * [5] S. HAR-PELED: A Practical Approach for Computing the Diameter of a Point Set. Proc. 17th ACM Sympos. Comput. Geom, 177-186, 2001.
 */
class DiameterAlgorithms {

	public:
		/**
		 * Approximates the diameter of the convex hull of a given set of points in the three-dimensional euclidean space.
		 *
		 * The algorithm is sensitive to the "hardness" of computing the diameter of the given input, and for most inputs
		 * it is able to compute the *exact* diameter extremely fast.
		 *
		 * For epsilon >= 0 a pair of points p,q\in points is computed, so that Delta >= |pq| >= (1-epsilon)*Delta,
		 * where Delta is the exact diameter of the given set of points.
		 *
		 * Running time: O((n+1/epsilon^3)*log 1/epsilon), for epsilon > 0
		 *               O(n^2*log n)                    , for epsilon = 0.
		 *
		 * The algorithm can also be used to compute the exact diameter by setting epsilon = 0.
		 * Although, in the worst case, the running time then is still quadratic, the running time is sensitive
		 * to the "hardness" of the input. Namely an instance of the diameter problem is hard, if almost all the
		 * pairs of points in the input have length close to the diameter of the point-set. However, in most
		 * inputs the diameter is quite distinctive and only a small fraction of all possible pairs are serious
		 * contenders to be a diametrical pair. The running time of the algorithm is related to this quantity and
		 * for most inputs it computes the exact diameter extremely fast.
		 *
		 * \param points	Set of points of which to compute a diametrical pair
		 * \param epsilon	Approximation factor >= 0
		 * \return			Pair of points p,q\in points as vector<Vector3d>, so that Delta >= |pq| >= (1-epsilon)*Delta,
		 *                  where Delta is the exact diameter of the given set of points.
		 */
		static std::vector<Vector3d> compute_diametrical_pair(
				const std::vector<Vector3d>& points, const double epsilon);

		static double compute_diameter(const std::vector<Vector3d>& points,
				const double epsilon);

		/**
		 * Computes the diameter of the convex hull of the given set of points in the three-dimensional euclidean space.
		 *
		 * Running time: O(n^2)
		 *
		 * \param points	Set of points of which to compute the diameter
		 * \return			Diameter of the convex hull of the given set of points
		 */
		static double compute_diameter_naive(
				const std::vector<Vector3d>& points);
};

#endif /* DIAMETER_H_ */

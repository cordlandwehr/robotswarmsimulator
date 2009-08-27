/*
 * diameter.h
 *
 *  Created on: 27.08.2009
 *      Author: dwarner
 *
 */

#ifndef DIAMETER_H_
#define DIAMETER_H_

#include "../Utilities/vector_arithmetics.h"


/**
 * \brief Calculates the diameter of the convex hull of a given set of points.
 * This is the same value as the maximum distance realized by two points in the set i.e., the
 * distance between two farthest points in the set.
 *
 * In literature the problem is referred to as farthest-pair-problem or diameter-problem.
 *
 * If w contains at most one point, then the distance 0 is returned.
 *
 * The straightforward algorithm to solve this problem compares each pair of points
 * and returns the maximum distance realized by two points in the set. This approach leads
 * to a Theta(n^2) time algorithm.
 *
 * A more efficient approach (compare [1,2]) leads to an algorithm with O(n log n) running time,
 * which is asymptotically optimal.
 *
 * The parameter "efficient" specifies whether to use the naive or the efficient algorithm.
 *
 * Literature:
 *
 * [1] PREPARATA, F. P., M. I. SHAMOS: Computational Geometry: An Introduction. Springer, 1985.
 * [2] ALSUWAIYEL, M. H.: Algorithms - Design Techniques and Analysis, World Scientific, 1999.
 */
const double calculate_diameter(const std::vector<Vector3d>& w);

const double calculate_diameter(const std::vector<Vector3d>& w, const bool efficient);

#endif /* DIAMETER_H_ */

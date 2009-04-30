/*
 * vector_difference_trimmer.cc
 *
 *  Created on: 07.02.2009
 *      Author: peter
 */

#include "vector_difference_trimmer.h"

bool VectorDifferenceTrimmer::modify_vector(Vector3d& input_vector, const Vector3d& reference_vector) {
	Vector3d difference_vector = input_vector - reference_vector;
	double rescale = length_/boost::numeric::ublas::norm_2(difference_vector);
	if (rescale < 1) {
		input_vector = reference_vector + rescale*difference_vector;
		return true;
	}
	return false;
}

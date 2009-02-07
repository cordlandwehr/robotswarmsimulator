/*
 *  vector_trimmer.cc
 *
 *  Created on: Feb 7, 2009
 *      Author: peter
 */

#include "vector_trimmer.h"


void VectorTrimmer::modify_vector(Vector3d& input_vector, const Vector3d& reference_vector) {
	double rescale = length_/boost::numeric::ublas::norm_2(input_vector);
	if (rescale < 1)
		input_vector *= rescale;
}

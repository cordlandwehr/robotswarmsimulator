/*
 * vector_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: peter
 */

#include "vector_randomizer.h"


void VectorRandomizer::modify_vector(Vector3d& input_vector, const Vector3d& reference_vector) {
	// generate random vector (each coordinate independently according to N(0, standard_deviation_))
	double x = distribution_generator_->get_value_normal();
	double y = distribution_generator_->get_value_normal();
	double z = distribution_generator_->get_value_normal();
	
	// add random vector to input vector
	input_vector(0) += x;
	input_vector(1) += y;
	input_vector(2) += z;
}

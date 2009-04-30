/*
 * vector_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: peter
 */

#ifndef VECTOR_RANDOMIZER_H_
#define VECTOR_RANDOMIZER_H_

#include "vector_modifier.h"

#include "../distribution_generator.h"


/**
 * \brief Vector modifier that randomizes the given vector.
 *
 * The vector is randomized by adding vector that is distributed according to a multi-dimensional standard normal
 * distribution. More exactly, each coordinate of the added vector is independently distributed according to
 * N(0, standard_deviation).
 */
class VectorRandomizer : public VectorModifier {
	friend class LoadMainProjectFileTest;
public:
	VectorRandomizer(unsigned int seed, double standard_deviation)
	: distribution_generator_(DistributionGenerator(seed)) {
		distribution_generator_.init_normal(0., standard_deviation);
	}

	/**
	 * \brief Randomizes the given input vector.
	 *
	 * Ignores reference vector.
	 *
	 * \return Returns true if the input vector was changed. Otherwise, false is returned.
	 */
	bool modify_vector(Vector3d& input_vector, const Vector3d& reference_vector);

private:
	DistributionGenerator distribution_generator_;
};

#endif /* VECTOR_RANDOMIZER_H_ */

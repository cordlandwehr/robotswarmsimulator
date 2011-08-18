/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
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

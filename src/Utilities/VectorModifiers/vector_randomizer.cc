/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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

#include "vector_randomizer.h"


bool VectorRandomizer::modify_vector(Vector3d& input_vector, const Vector3d& reference_vector) {
	// generate random vector (each coordinate independently according to N(0, standard_deviation_))
	double x = distribution_generator_.get_value_normal();
	double y = distribution_generator_.get_value_normal();
	double z = distribution_generator_.get_value_normal();
	
	// add random vector to input vector
	input_vector(0) += x;
	input_vector(1) += y;
	input_vector(2) += z;
	return (x != 0 || y != 0 || z != 0);
}

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

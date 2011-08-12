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

#ifndef VECTOR_MODIFIER_H_
#define VECTOR_MODIFIER_H_

#include "../vector_arithmetics.h"


/**
 * \brief Abstract base class for vector modifiers.
 *
 * It is mainly designed to be used by VectorRequestHandler, but may be useful in other situations too.
 * \see modify_vector
 */
class VectorModifier {
public:
	VectorModifier() { }
	virtual ~VectorModifier() { }

	/**
	 * \brief Modifies the given input vector.
	 *
	 * Subclasses overwrite this method such that the given input vector is modified according to the subclass
	 * specification. The reference_vector is used to give vector modifiers additional information about the original
	 * vector (for example to make sure that difference between input and reference vector does not become too large).
	 * However, subclasses may ignore it.
	 *
	 * The return value has to indicate wether the input vector has actually been modified (true) or not (false).
	 */
	virtual bool modify_vector(Vector3d& input_vector, const Vector3d& reference_vector) = 0;
};

#endif /* VECTOR_MODIFIER_H_ */

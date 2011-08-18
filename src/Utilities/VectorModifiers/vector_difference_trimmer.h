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
 * vector_difference_trimmer.h
 *
 *  Created on: 07.02.2009
 *      Author: peter
 */

#ifndef VECTOR_DIFFERENCE_TRIMMER_H_
#define VECTOR_DIFFERENCE_TRIMMER_H_

#include "vector_modifier.h"


/**
 * \brief Vector modifier that changes the input vector if the difference between the input and reference vector exceeds
 *        a specific length.
 *
 * More exactly, if length of 'input - reference' exceeds 'length', input is moved in 'input - reference' direction until
 * the length of 'input-reference' matches exactly 'length'.
 */
class VectorDifferenceTrimmer : public VectorModifier {
	friend class LoadMainProjectFileTest;
public:
	VectorDifferenceTrimmer(double length) : length_(length) { }

	/**
	 * \brief Trims the input vector to have at most the configured length.
	 *
	 * Ignores reference vector.
	 *
	 * \return Returns true if the input vector was changed. Otherwise, false is returned.
	 */
	bool modify_vector(Vector3d& input_vector, const Vector3d& reference_vector);


	double length() const { return length_; }
	void set_length(double value) { length_ = value; }

private:
	double length_;
};

#endif /* VECTOR_DIFFERENCE_TRIMMER_H_ */

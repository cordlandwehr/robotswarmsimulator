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
	 */
	void modify_vector(Vector3d& input_vector, const Vector3d& reference_vector);


	double length() const { return length_; }
	void set_length(double value) { length_ = value; }

private:
	double length_;
};

#endif /* VECTOR_DIFFERENCE_TRIMMER_H_ */

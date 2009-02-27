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
	 */
	virtual void modify_vector(Vector3d& input_vector, const Vector3d& reference_vector) = 0;
};

#endif /* VECTOR_MODIFIER_H_ */

/*
 * vector_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: peter
 */

#ifndef VECTOR_MODIFIER_H_
#define VECTOR_MODIFIER_H_

#include "../Utilities/vector3d.h"


/**
 * \brief Abstract base class for vector modifiers.
 * \see modify_vector
 */
class VectorModifier {
public:
	VectorModifier() {}
	virtual ~VectorModifier() {}
	
	/**
	 * \brief Modifies the given input vector.
	 *
	 * Subclasses overwrite this method such that the given input vector is modified according to the subclass
	 * specification. The reference_vector is useed by request handlers, to give vector modifiers information about the
	 * original requested vector.
	 */
	virtual void modify_vector(Vector3d& input_vector, const Vector3d& reference_vector) = 0;
};

#endif /* VECTOR_MODIFIER_H_ */

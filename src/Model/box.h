/**
 * \class	Box
 * \author	Martina Hüllmann
 * \brief	Denotes an box-obstacle
 */

#ifndef BOX_H_
#define BOX_H_

#include "obstacle.h"

class Box : public Obstacle {
public:
	Box();
	virtual ~Box();

	/**
	 * Returns the length of the box.
	 * \return Length of the box.
	 */
	double length() const;

	/**
	 * Sets the length of the box to the given value.
	 * \param New length of the box.
	 */
	void set_length(double new_length);

	/**
	 * Returns the depth of the box.
	 * \return Depth of the box.
	 */
	double depth() const;

	/**
	 * Sets the depth of the box to the given value.
	 * \param New depth of the box.
	 */
	void set_depth(double new_depth);

private:
	double depth_;
	double length_;
};

#endif /* BOX_H_ */

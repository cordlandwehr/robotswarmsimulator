/**
 * \class	Box
 * \author	Martina Hüllmann
 * \brief	Denotes a sphere-obstacle
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "obstacle.h"

class Sphere : public Obstacle {
public:
	Sphere();
	virtual ~Sphere();

	/**
	 * Returns the radius of the sphere.
	 * \return Radius of the sphere.
	 */
	double radius() const;

	/**
	 * Sets the radius of the sphere to the given value.
	 * \param New radius of the sphere.
	 */
	void set_radius(double new_radius);

private:
	double radius_;
};

#endif /* SPHERE_H_ */

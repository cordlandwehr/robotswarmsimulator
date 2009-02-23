/**
 * \class	Sphere
 * \author	Martina Hüllmann
 * \brief	Denotes a sphere-obstacle
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "obstacle.h"


class Sphere : public Obstacle {
public:
	Sphere(boost::shared_ptr<Identifier> id,
	       boost::shared_ptr<Vector3d> position,
	       double radius);
	Sphere(boost::shared_ptr<Identifier> id,
           boost::shared_ptr<Vector3d> position,
	       boost::shared_ptr<MarkerInformation> marker_information,
	       double radius);
	~Sphere();

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

	/**
	 * Checks whether the given point is contained in the obstacle.
	 * \param Pointer to vector of point to check whether it's
	 *        contained in the obstacle.
	 */
	bool contains_point(boost::shared_ptr<Vector3d> point) const;

	virtual boost::shared_ptr<WorldObject> clone() const;

	double max_dimension() const{ return radius_; }
private:
	double radius_;
};

#endif /* SPHERE_H_ */

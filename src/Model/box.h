/**
 * \class	Box
 * \author	Martina Hüllmann
 * \brief	Denotes an box-obstacle
 */

#ifndef BOX_H_
#define BOX_H_

#include "obstacle.h"
#include "../Utilities/vector_arithmetics.h"


class Box : public Obstacle {
public:
	Box(boost::shared_ptr<Identifier> id,
		boost::shared_ptr<Vector3d> position,
		double depth,
		double width,
		double height);
	Box(boost::shared_ptr<Identifier> id,
	    boost::shared_ptr<Vector3d> position,
        boost::shared_ptr<MarkerInformation> marker_information,
        double depth,
       	double width,
       	double height);
	~Box();

	/**
	 * Returns the length of the box.
	 * \return Length of the box.
	 */
	double height() const;

	/**
	 * Sets the height of the box to the given value.
	 * \param New height of the box.
	 */
	void set_height(double new_height);

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

	/**
	 * Returns the width of the box.
	 * \return Width of the box.
	 */
	double width() const;

	/**
	 * Sets the width of the box to the given value.
	 * \param New width of the box.
	 */
	void set_width(double new_width);

	/**
	 * Checks whether the given point is contained in the obstacle.
	 * \param Pointer to vector of point to check whether it's contained
	 * 	      in the obstacle.
	 */
	bool contains_point(boost::shared_ptr<Vector3d> point) const;

	virtual boost::shared_ptr<WorldObject> clone() const;

	double max_dimension() const;

private:
	/**
	 * \var Depth (in z-direction) of the box.
	 */
	double depth_;
	/**
	 * \var Width (in x-direction) of the box.
	 */
	double width_;
	/**
	 * \var Height in (y-direction) of the box.
	 */
	double height_;
};

#endif /* BOX_H_ */

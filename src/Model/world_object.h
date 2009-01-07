/**
 * \class	WorldObject
 * \author	Martina Hüllmann
 * \brief 	An instance of this class denotes an object
 * 			(robot, marker, obstacle) in the world.
 */
#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

#include <boost/smart_ptr.hpp>
#include <vector>

#include "marker_information.h"

class WorldObject {
public:
	WorldObject();
	virtual ~WorldObject();

	/**
	 * Sets the marker information of this object.
	 * \param a shared pointer to the new marker information
	 */
	void set_marker_information(MarkerInformation * new_marker_information);

	/**
	 * Returns a constant reference to the marker information of this object.
	 * \return Constant reference to the marker information of this object.
	 */
	const boost::shared_ptr<MarkerInformation>& marker_information() const;

	/**
	 * Sets the position of this object
	 * \param Position vector
	 */
	void set_position(std::vector<double> new_position);

	/**
	 * Returns a the position vector this object.
	 * \return Position vector this object.
	 */
	const std::vector<double> position() const;

private:
	/**
	 * Information about the marker an instance of WorldObject may contain
	 */
	boost::shared_ptr<MarkerInformation> marker_information_;

protected:
	/**
	 * Position of the world object in the world.
	 * This point always is the point in the center of the object
	 * This information is needed for objects with a volume > 0.
	 */
	std::vector<double> position_;
};

#endif /* WORLD_OBJECT_H_ */

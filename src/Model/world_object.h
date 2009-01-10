/**
 * \class	WorldObject
 * \author	Martina Hüllmann
 * \brief 	An instance of this class denotes an object
 * 			(robot, marker, obstacle) in the world.
 */
#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

#include <boost/smart_ptr.hpp>
#include "../Utilities/vector3d.h"

class MarkerInformation;

class WorldObject {
public:
	WorldObject();
	virtual ~WorldObject();

	/**
	 * Sets the marker information of this object.
	 * \param a shared pointer to the new marker information
	 */
	void set_marker_information(boost::shared_ptr<MarkerInformation> new_marker_information);

	/**
	 * Returns a constant reference to the marker information of this object.
	 * \return Constant reference to the marker information of this object.
	 */
	const MarkerInformation& marker_information() const;

	/**
	 * Sets the position of this object.
	 * \param Pointer to new position vector.
	 */
	void set_position(boost::shared_ptr<Vector3d> new_position) {position_ = new_position;};

	/**
	 * Returns constant reference to position vector.
	 * \return Constant reference to position vector.
	 */
	const Vector3d & position() const {return *position_;};

private:
	/**
	 * Information about the marker an instance of WorldObject may contain
	 */
	boost::shared_ptr<MarkerInformation> marker_information_;

protected:
	/**
	 * Position of the world object in the world.
	 * This point always is the point in the center of the object
	 */
	boost::shared_ptr<Vector3d> position_;
};

#endif /* WORLD_OBJECT_H_ */

/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
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

/**
 * \class	WorldObject
 * \author	Martina Hüllmann
 * \brief 	An instance of this class denotes an object
 * 			(robot, marker, obstacle) in the world.
 */
#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

#include <boost/shared_ptr.hpp>
#include "../Utilities/vector_arithmetics.h"
#include "marker_information.h"

class Identifier;

class WorldObject {
public:
	WorldObject(boost::shared_ptr<Identifier> id,
	            boost::shared_ptr<Vector3d> position,
	            boost::shared_ptr<MarkerInformation> marker_information = boost::shared_ptr<MarkerInformation>(new MarkerInformation()));
	virtual ~WorldObject();
	WorldObject(const WorldObject& rhs);

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
	 * Returns a reference to the marker information of this object.
	 * \return Mutable reference to the marker information of this object.
	 */
	MarkerInformation& marker_information();

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

	/**
	 * Returns constant reference to the object's id.
	 * \return Constant reference to the object's id.
	*/
	const boost::shared_ptr<Identifier>& id() const;

	/**
	 * Clones this object and returns a shared ptr to the cloned object.
	 * typeid(*this) == typeid(*clone)
	 * @return shared ptr to the cloned object
	 */
	virtual boost::shared_ptr<WorldObject> clone() const;

private:
	boost::shared_ptr<Identifier> id_;
protected:
	/**
	* Position of the world object in the world.
	* This point always is the point in the center of the object
	*/
	boost::shared_ptr<Vector3d> position_;
private:
	/**
	 * Information about the marker an instance of WorldObject may contain
	 */
	boost::shared_ptr<MarkerInformation> marker_information_;

};

#endif /* WORLD_OBJECT_H_ */

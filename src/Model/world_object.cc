/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

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

#include "world_object.h"
#include "marker_information.h"
#include "identifier.h"

WorldObject::WorldObject(boost::shared_ptr<Identifier> id,
                         boost::shared_ptr<Vector3d> position,
                         boost::shared_ptr<MarkerInformation> marker_information)
                         : id_(id), position_(position), marker_information_(marker_information)
{

}

WorldObject::~WorldObject() {
	
}

WorldObject::WorldObject(const WorldObject& rhs) {
	if(rhs.id_) {
		this->id_ = rhs.id_->clone();
	}
	if(rhs.position_) {
		this->position_.reset(new Vector3d(*rhs.position_));
	}
	if(rhs.marker_information_) {
		this->marker_information_.reset(new MarkerInformation(*rhs.marker_information_));
	}
}

const MarkerInformation& WorldObject::marker_information() const {
	return *marker_information_;
}

MarkerInformation& WorldObject::marker_information() {
	return *marker_information_;
}

void WorldObject::set_marker_information(boost::shared_ptr<MarkerInformation> new_marker_information) {
	marker_information_ = new_marker_information;
}

const boost::shared_ptr<Identifier>& WorldObject::id() const {
	return id_;
}

boost::shared_ptr<WorldObject> WorldObject::clone() const {
	return boost::shared_ptr<WorldObject>(new WorldObject(*this));
}

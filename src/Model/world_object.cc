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

void WorldObject::set_marker_information(boost::shared_ptr<MarkerInformation> new_marker_information) {
	marker_information_ = new_marker_information;
}

const boost::shared_ptr<Identifier>& WorldObject::id() const {
	return id_;
}

boost::shared_ptr<WorldObject> WorldObject::clone() const {
	return boost::shared_ptr<WorldObject>(new WorldObject(*this));
}

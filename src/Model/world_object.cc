#include "world_object.h"

WorldObject::WorldObject() {
	// TODO Auto-generated constructor stub

}

WorldObject::~WorldObject() {
	// TODO Auto-generated destructor stub
}

const boost::shared_ptr<MarkerInformation>& WorldObject::marker_information() const {
	return marker_information_;
}

void WorldObject::set_marker_information (MarkerInformation * new_marker_information) {
	marker_information_.reset(new_marker_information);
}

const std::vector<double> WorldObject::position() const {
	return position_;
}

void WorldObject::set_position(std::vector<double> new_position) {
	position_ = new_position;
}

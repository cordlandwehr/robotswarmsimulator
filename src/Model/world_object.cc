#include "world_object.h"
#include "marker_information.h"

WorldObject::WorldObject() {
	// TODO Auto-generated constructor stub

}

WorldObject::~WorldObject() {
	// TODO Auto-generated destructor stub
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

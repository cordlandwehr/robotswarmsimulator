#include "world_information.h"

WorldInformation::WorldInformation() {
	// TODO Auto-generated destructor stub
}

WorldInformation::~WorldInformation() {
	// TODO Auto-generated destructor stub
}

const vector<boost::shared_ptr<WorldObject> >& WorldInformation::markers() const {
	return markers_;
}

void WorldInformation::add_marker(boost::shared_ptr<WorldObject> new_marker) {
	markers_.push_back(new_marker);
}

const vector<boost::shared_ptr<Obstacle> >& WorldInformation::obstacles() const {
	return obstacles_;
}

void WorldInformation::add_obstacle(boost::shared_ptr<Obstacle> new_obstacle) {
	obstacles_.push_back(new_obstacle);
}

const vector<boost::shared_ptr<Robot> >& WorldInformation::robots() const {
	return robots_;
}

void WorldInformation::add_robot(boost::shared_ptr<Robot> new_robot) {
	robots_.push_back(new_robot);
}

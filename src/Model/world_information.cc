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

const vector<boost::shared_ptr<RobotData> >& WorldInformation::robot_datas() const {
	return robot_datas_;
}

void WorldInformation::add_robot_data(boost::shared_ptr<RobotData> new_robot_data) {
	robot_datas_.push_back(new_robot_data);
}

int WorldInformation::time() const {
	return time_;
}

const boost::shared_ptr<RobotData>& WorldInformation::get_according_robot_data(Identifier * id) const {
	//TODO(martinah) maybe check if robot_datas_[id->id()] exists? (should exist!)
	return robot_datas_[id->id()];
}


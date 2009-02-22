#include "world_information.h"
#include "identifier.h"
#include "robot_identifier.h"
#include "world_object.h"
#include "robot_data.h"
#include "obstacle.h"

#include <iostream>
#include <boost/foreach.hpp>
#include <cassert>


WorldInformation::WorldInformation() {
	// TODO Auto-generated destructor stub
}

WorldInformation::~WorldInformation() {
	// TODO Auto-generated destructor stub
}

WorldInformation::WorldInformation(const WorldInformation& rhs) : time_(rhs.time_){
	this->markers_.reserve(rhs.markers_.size());
	BOOST_FOREACH(const boost::shared_ptr<WorldObject>& marker, rhs.markers_) {
		if(marker) {
			this->markers_.push_back(marker->clone());
		}
		else {
			this->markers_.push_back(boost::shared_ptr<WorldObject>());
		}
	}

	this->obstacles_.reserve(rhs.obstacles_.size());
	BOOST_FOREACH(const boost::shared_ptr<Obstacle>& obstacle, rhs.obstacles_) {
		if(obstacle) {
			this->obstacles_.push_back(boost::static_pointer_cast<Obstacle>(obstacle->clone()));
		}
		else {
			this->obstacles_.push_back(boost::shared_ptr<Obstacle>());
		}
	}

	this->robot_data_.reserve(rhs.robot_data_.size());
	BOOST_FOREACH(const boost::shared_ptr<RobotData>& robot, rhs.robot_data_) {
		if(robot) {
			this->robot_data_.push_back(boost::static_pointer_cast<RobotData>(robot->clone()));
		}
		else {
			this->robot_data_.push_back(boost::shared_ptr<RobotData>());
		}
	}
}

const vector<boost::shared_ptr<WorldObject> >& WorldInformation::markers() const {
	return markers_;
}

vector<boost::shared_ptr<WorldObject> >& WorldInformation::markers() {
	return markers_;
}

void WorldInformation::add_marker(boost::shared_ptr<WorldObject> new_marker) {
	markers_.push_back(new_marker);
}

const vector<boost::shared_ptr<Obstacle> >& WorldInformation::obstacles() const {
	return obstacles_;
}

vector<boost::shared_ptr<Obstacle> >& WorldInformation::obstacles() {
	return obstacles_;
}

void WorldInformation::add_obstacle(boost::shared_ptr<Obstacle> new_obstacle) {
	obstacles_.push_back(new_obstacle);
}

const vector<boost::shared_ptr<RobotData> >& WorldInformation::robot_data() const {
	return robot_data_;
}

vector<boost::shared_ptr<RobotData> >& WorldInformation::robot_data() {
	return robot_data_;
}

void WorldInformation::add_robot_data(boost::shared_ptr<RobotData> new_robot_data) {
	robot_data_.push_back(new_robot_data);
}

int WorldInformation::time() const {
	return time_;
}

const RobotData& WorldInformation::get_according_robot_data(boost::shared_ptr<RobotIdentifier> id) const {
	assert(id->id() < robot_data_.size());
	return *(robot_data_[id->id()]);
}

RobotData& WorldInformation::get_according_robot_data(boost::shared_ptr<RobotIdentifier> id) {
	assert(id->id() < robot_data_.size());
	return *(robot_data_[id->id()]);
}

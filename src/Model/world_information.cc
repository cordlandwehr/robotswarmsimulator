#include "world_information.h"
#include "identifier.h"
#include "marker_identifier.h"
#include "robot_identifier.h"
#include "world_object.h"
#include "robot_data.h"
#include "obstacle.h"

#include <iostream>
#include <boost/foreach.hpp>
#include <cassert>


WorldInformation::WorldInformation() {
}

WorldInformation::~WorldInformation() {
	
	

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

	for(std::map<size_t, boost::shared_ptr<Edge> >::const_iterator it = rhs.edges_.begin(); it != rhs.edges_.end(); it++){
		this->edges_.insert(std::pair<std::size_t, boost::shared_ptr<Edge> >(it->first, boost::static_pointer_cast<Edge>(it->second->clone())));
	}
}

const std::vector<boost::shared_ptr<WorldObject> >& WorldInformation::markers() const {
	return markers_;
}

std::vector<boost::shared_ptr<WorldObject> >& WorldInformation::markers() {
	return markers_;
}


void WorldInformation::add_marker(boost::shared_ptr<WorldObject> new_marker) {
	assert(new_marker->id()->id() == markers_.size());
	markers_.push_back(new_marker);
}

void WorldInformation::set_marker_data(std::vector<boost::shared_ptr<WorldObject> > new_markers) {
	markers_ = new_markers;
}

const std::vector<boost::shared_ptr<Obstacle> >& WorldInformation::obstacles() const {
	return obstacles_;
}

std::vector<boost::shared_ptr<Obstacle> >& WorldInformation::obstacles() {
	return obstacles_;
}

void WorldInformation::add_obstacle(boost::shared_ptr<Obstacle> new_obstacle) {
	assert(new_obstacle->id()->id() == obstacles_.size());
	obstacles_.push_back(new_obstacle);
}

void WorldInformation::set_obstacle_data(std::vector<boost::shared_ptr<Obstacle> > new_obstacles) {
	obstacles_ = new_obstacles;
}

const std::vector<boost::shared_ptr<RobotData> >& WorldInformation::robot_data() const {
	return robot_data_;
}

std::vector<boost::shared_ptr<RobotData> >& WorldInformation::robot_data() {
	return robot_data_;
}

void WorldInformation::add_robot_data(boost::shared_ptr<RobotData> new_robot_data) {
	assert(new_robot_data->id()->id() == robot_data_.size());
	robot_data_.push_back(new_robot_data);
}

void WorldInformation::set_robot_data(std::vector<boost::shared_ptr<RobotData> > new_robot_data) {
	robot_data_ = new_robot_data;
}

const std::map<std::size_t, boost::shared_ptr<Edge> >& WorldInformation::edges() const {
	return edges_;
}

std::map<std::size_t, boost::shared_ptr<Edge> >& WorldInformation::edges() {
	return edges_;
}

void WorldInformation::add_edge(boost::shared_ptr<Edge> new_edge) {
	edges_.insert(std::pair<std::size_t, boost::shared_ptr<Edge> >(new_edge->id()->id(), new_edge));
}

void WorldInformation::set_edge_data(std::map<std::size_t, boost::shared_ptr<Edge> > new_edges) {
	edges_ = new_edges;
}

bool WorldInformation::remove_edge(boost::shared_ptr<Edge> edge){
	return edges_.erase(edge->id()->id());
}

const std::map<std::size_t, boost::shared_ptr<Message> >& WorldInformation::messages() const {
	return messages_;
}

std::map<std::size_t, boost::shared_ptr<Message> >& WorldInformation::messages() {
	return messages_;
}

void WorldInformation::add_message(boost::shared_ptr<Edge> new_message) {
	messages_.insert(std::pair<std::size_t, boost::shared_ptr<Message> >(new_message->id()->id(), new_message));
}

void WorldInformation::set_message_data(std::map<std::size_t, boost::shared_ptr<Message> > new_messages) {
	messages_ = new_messages;
}

bool WorldInformation::remove_message(boost::shared_ptr<Edge> message){
	return messages_.erase(message->id()->id());
}

int WorldInformation::time() const {
	return time_;
}

const WorldObject& WorldInformation::get_according_marker(const MarkerIdentifier& id) const {
	assert(id.id() < markers_.size());
	return *(markers_[id.id()]);
}

WorldObject& WorldInformation::get_according_marker(const MarkerIdentifier& id) {
	assert(id.id() < markers_.size());
	return *(markers_[id.id()]);
}

const RobotData& WorldInformation::get_according_robot_data(boost::shared_ptr<RobotIdentifier> id) const {
	assert(id->id() < robot_data_.size());
	return *(robot_data_[id->id()]);
}

RobotData& WorldInformation::get_according_robot_data(boost::shared_ptr<RobotIdentifier> id) {
	assert(id->id() < robot_data_.size());
	return *(robot_data_[id->id()]);
}

boost::shared_ptr<const RobotData> WorldInformation::get_according_robot_data_ptr(boost::shared_ptr<RobotIdentifier> id) const {
	assert(id->id() < robot_data_.size());
	return robot_data_[id->id()];
}

boost::shared_ptr<RobotData> WorldInformation::get_according_robot_data_ptr(boost::shared_ptr<RobotIdentifier> id) {
	assert(id->id() < robot_data_.size());
	return robot_data_[id->id()];
}

boost::shared_ptr<const Edge> WorldInformation::get_according_edge(boost::shared_ptr<EdgeIdentifier> id) const {
	std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator it = edges_.find(id->id());
	assert(it != edges_.end());
	return it->second;
}

boost::shared_ptr<Edge> WorldInformation::get_according_edge(boost::shared_ptr<EdgeIdentifier> id) {
	std::map<std::size_t, boost::shared_ptr<Edge> >::iterator it = edges_.find(id->id());
	assert(it != edges_.end());
	return it->second;
}

boost::shared_ptr<const Message> WorldInformation::get_according_message(boost::shared_ptr<MessageIdentifier> id) const {
	std::map<std::size_t, boost::shared_ptr<Message> >::const_iterator it = messages_.find(id->id());
	assert(it != messages_.end());
	return it->second;
}

boost::shared_ptr<Message> WorldInformation::get_according_message(boost::shared_ptr<MessageIdentifier> id) {
	std::map<std::size_t, boost::shared_ptr<Message> >::iterator it = messages_.find(id->id());
	assert(it != messages_.end());
	return it->second;
}

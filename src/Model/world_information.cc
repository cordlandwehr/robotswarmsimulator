/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
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

#include "world_information.h"
#include "identifier.h"
#include "marker_identifier.h"
#include "robot_identifier.h"
#include "message_identifier.h"
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

	//this->robot_data_.reserve(rhs.robot_data_.size());
	for (std::map< int, boost::shared_ptr < RobotData> >::const_iterator it = rhs.robot_data_.begin(); it != rhs.robot_data_.end(); ++it) {
		this->robot_data_[it->first] = boost::static_pointer_cast<RobotData>(it->second->clone());
	}
	  
	/*BOOST_FOREACH(const boost::shared_ptr<RobotData>& robot, rhs.robot_data_) {
		if(robot) {
			this->robot_data_[robot->id()->id()] = boost::static_pointer_cast<RobotData>(robot->clone());
		}
		//else { //TODO asetzer what sense does this make?
		//	this->robot_data_.push_back(boost::shared_ptr<RobotData>());
		//}
	}*/

	for(std::map<size_t, boost::shared_ptr<Edge> >::const_iterator it = rhs.edges_.begin(); it != rhs.edges_.end(); it++){
		this->edges_.insert(std::pair<std::size_t, boost::shared_ptr<Edge> >(it->first, boost::static_pointer_cast<Edge>(it->second->clone())));
	}

	for(std::map<size_t, boost::shared_ptr<Message> >::const_iterator it = rhs.messages_.begin(); it != rhs.messages_.end(); it++){
		this->messages_.insert(std::pair<std::size_t, boost::shared_ptr<Message> >(it->first, boost::static_pointer_cast<Message>(it->second->clone())));
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

const std::map<int, boost::shared_ptr<RobotData> >& WorldInformation::robot_data() const {
	return robot_data_;
}

void WorldInformation::robot_data_to_vector(std::vector<boost::shared_ptr<RobotData> >& vec) const {
  for (std::map< int, boost::shared_ptr < RobotData> >::const_iterator it = robot_data_.begin(); it != robot_data_.end(); ++it) {
	vec.push_back(it->second);
  }  
}

std::map<int, boost::shared_ptr<RobotData> >& WorldInformation::robot_data() {
	return robot_data_;
}

void WorldInformation::add_robot_data(boost::shared_ptr<RobotData> new_robot_data) {
	robot_data_[new_robot_data->id()->id()] = new_robot_data;
}

void WorldInformation::set_robot_data(std::map<int, boost::shared_ptr<RobotData> > new_robot_data) {
	robot_data_ = new_robot_data;
}

void WorldInformation::remove_robot_data(boost::shared_ptr<RobotIdentifier> robot_identifier) {
	size_t robot_id = robot_identifier->id();
	
	//remove the node itself	
	robot_data_.erase(robot_id);

	//remove any edges to or from this node
	std::map< size_t, boost::shared_ptr < Edge> >::iterator it_e = edges_.begin();
	while( it_e != edges_.end() ) {
		std::map< size_t, boost::shared_ptr < Edge> >::iterator old_it_e = it_e++;
		if (old_it_e->second->robot1()->id() == robot_id || old_it_e->second->robot2()->id() == robot_id) {
			edges_.erase(old_it_e);
		}
	}

	//remove message in this node's message queue
	std::map< size_t, boost::shared_ptr < Message> >::iterator it_m = messages_.begin();
	while( it_m != messages_.end() ) {
		std::map< size_t, boost::shared_ptr < Message> >::iterator old_it_m = it_m++;
		if (old_it_m->second->receiver()->id() == robot_id) {
			messages_.erase(old_it_m);
		}
	}	
	
	
}

const std::map<std::size_t, boost::shared_ptr<Edge> >& WorldInformation::edges() const {
	return edges_;
}

std::map<std::size_t, boost::shared_ptr<Edge> >& WorldInformation::edges() {
	return edges_;
}

void WorldInformation::add_edge(boost::shared_ptr<Edge> new_edge) {
	RobotData& rd1 = get_according_robot_data(new_edge->robot1());
	RobotData& rd2 = get_according_robot_data(new_edge->robot2());

	// add requested edge to world_information and to adjacency list of robots
	rd1.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(new_edge->id()));
	if(rd1.id()->id() != rd2.id()->id()) // do not add loop edges twice
		rd2.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(new_edge->id()));

	edges_.insert(std::pair<std::size_t, boost::shared_ptr<Edge> >(new_edge->id()->id(), new_edge));
}

void WorldInformation::set_edge_data(std::map<std::size_t, boost::shared_ptr<Edge> > new_edges) {
	edges_ = new_edges;
}

void WorldInformation::remove_edge(boost::shared_ptr<Edge> edge){
	RobotData& rd1 = get_according_robot_data(edge->robot1());
	RobotData& rd2 = get_according_robot_data(edge->robot2());

	// remove requested edge from world_information and from adjacency list of robots
	rd1.remove_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(edge->id()));
	rd2.remove_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(edge->id()));  

	// remove from world information
	edges_.erase(edge->id()->id());
}

const std::map<std::size_t, boost::shared_ptr<Message> >& WorldInformation::messages() const {
	return messages_;
}

std::map<std::size_t, boost::shared_ptr<Message> >& WorldInformation::messages() {
	return messages_;
}

void WorldInformation::add_message(boost::shared_ptr<Message> new_message) {
	// add message to queue of receiver
	RobotData& rd_receiver = get_according_robot_data(new_message->receiver());
	rd_receiver.push_back_message(boost::dynamic_pointer_cast<MessageIdentifier>(new_message->id()));  
  
	// add message to system
	messages_.insert(std::pair<std::size_t, boost::shared_ptr<Message> >(new_message->id()->id(), new_message));
}

void WorldInformation::set_message_data(std::map<std::size_t, boost::shared_ptr<Message> > new_messages) {
	messages_ = new_messages;
}

void WorldInformation::remove_message(boost::shared_ptr<Message> message){
	if(robot_exists(message->receiver())){
		//check whether the message is still in the receiver's queue
		RobotData& rd = get_according_robot_data(message->receiver());
		for(std::size_t i=0; i<rd.get_number_of_messages(); ++i){
			if(rd.get_message(i)->id() == message->id()->id()){
				rd.remove_message(i);
				break;
			}
		}
	}
  
	//now completely remove it from the system
	messages_.erase(message->id()->id());
}

int WorldInformation::time() const {
	return time_;
}

bool WorldInformation::robot_exists(boost::shared_ptr<RobotIdentifier> id) const {
	std::map<int, boost::shared_ptr<RobotData> >::const_iterator it;
	for(it = robot_data_.begin(); it != robot_data_.end(); it++) {
		if(it->first == id->id())
			break;
	}
	if(it != robot_data_.end()){
		return true;
	} else {
		return false;
	}
}

bool WorldInformation::edge_exists(boost::shared_ptr<EdgeIdentifier> id) const {
	std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator it;
	for(it = edges_.begin(); it != edges_.end(); it++) {
		if(it->first == id->id())
			break;
	}
	if(it != edges_.end()){
		return true;
	} else {
		return false;
	}
}

bool WorldInformation::message_exists(boost::shared_ptr<MessageIdentifier> id) const {
	std::map<std::size_t, boost::shared_ptr<Message> >::const_iterator it;
	for(it = messages_.begin(); it != messages_.end(); it++) {
		if(it->first == id->id())
			break;
	}
	if(it != messages_.end()){
		return true;
	} else {
		return false;
	}
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
	return *(robot_data_.at(id->id()));
}

RobotData& WorldInformation::get_according_robot_data(boost::shared_ptr<RobotIdentifier> id) {
	return *(robot_data_.at(id->id()));
}

boost::shared_ptr<const RobotData> WorldInformation::get_according_robot_data_ptr(boost::shared_ptr<RobotIdentifier> id) const {
	return robot_data_.at(id->id());
}

boost::shared_ptr<RobotData> WorldInformation::get_according_robot_data_ptr(boost::shared_ptr<RobotIdentifier> id) {
	return robot_data_.at(id->id());
}

boost::shared_ptr<const Edge> WorldInformation::get_according_edge(boost::shared_ptr<EdgeIdentifier> id) const {
	std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator it;
	for(it = edges_.begin(); it != edges_.end(); it++) {
		if(it->first == id->id())
			break;
	}
	assert(it != edges_.end());
	return it->second;
}

boost::shared_ptr<Edge> WorldInformation::get_according_edge(boost::shared_ptr<EdgeIdentifier> id) {
	std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator it;
	for(it = edges_.begin(); it != edges_.end(); it++) {
		if(it->first == id->id())
			break;
	}
	assert(it != edges_.end());
	return it->second;
}

boost::shared_ptr<const Message> WorldInformation::get_according_message(boost::shared_ptr<MessageIdentifier> id) const {
	std::map<std::size_t, boost::shared_ptr<Message> >::const_iterator it;
	for(it = messages_.begin(); it != messages_.end(); it++) {
		if(it->first == id->id())
			break;
	}
	assert(it != messages_.end());
	return it->second;
}

boost::shared_ptr<Message> WorldInformation::get_according_message(boost::shared_ptr<MessageIdentifier> id) {
	std::map<std::size_t, boost::shared_ptr<Message> >::const_iterator it;
	for(it = messages_.begin(); it != messages_.end(); it++) {
		if(it->first == id->id())
			break;
	}
	assert(it != messages_.end());
	return it->second;
}

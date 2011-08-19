//
//  world_information_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//


#include "world_information_wrapper.h"

#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Model/marker_identifier.h"
#include "../Requests/marker_request.h"
#include "../Model/message.h"
#include "../Model/message_identifier.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/graph_concepts.hpp>

namespace LuaWrapper {
 
WorldInformationWrapper::WorldInformationWrapper () {}

// initialization of static members
boost::shared_ptr<WorldInformation> WorldInformationWrapper::world_information_;
std::set< boost::shared_ptr<Request> > WorldInformationWrapper::request_set_;
std::map< std::size_t, boost::shared_ptr<EdgeIdentifier> > WorldInformationWrapper::edge_identifiers_;
std::map< std::size_t, boost::shared_ptr<MessageIdentifier> > WorldInformationWrapper::message_identifiers_;
std::map< std::size_t, boost::shared_ptr<RobotIdentifier> > WorldInformationWrapper::robot_identifiers_; 

WorldInformationWrapper::~WorldInformationWrapper () {}

void
WorldInformationWrapper::add_edge(std::size_t source, std::size_t target, const std::string &type) {
  MarkerInformationWrapper marker;
  add_edge(source, target, marker, type);
} 

void
WorldInformationWrapper::add_edge(std::size_t source, std::size_t target,
				  MarkerInformationWrapper marker, const std::string &type) {
  // check the given IDs
  check_mapping(robot_identifiers_, source);
  check_mapping(robot_identifiers_, target);
  // get robot IDs
  boost::shared_ptr<RobotIdentifier> source_robot = robot_identifiers_[source];
  boost::shared_ptr<RobotIdentifier> target_robot = robot_identifiers_[target]; 
  // create new edge (decide type depending on given string)
  std::string type_lower(type);
  boost::to_lower(type_lower);
  boost::shared_ptr<Edge> edge;
  if (type_lower == "directed") {
    edge.reset(new DirectedEdge(source_robot, target_robot));
  } else {
    edge.reset(new UndirectedEdge(source_robot, target_robot));
  }
  // create and assign new MarkerInforamtion object
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  edge->set_marker_information(new_marker);
  // add edge to world
  world_information_->add_edge(edge);
}  

template<typename T> void 
WorldInformationWrapper::check_mapping(const std::map<std::size_t, boost::shared_ptr<T> >& map, std::size_t key) {
  // get iterator for given key
  typename std::map<std::size_t, boost::shared_ptr<T> >::const_iterator it;
  it = map.find(key);
  // check entry
  if (it == map.end()) {
    throw std::invalid_argument("The given ID is unknown to the Lua Wrapper. Possible reasons: The ID does not exists. The ID has not been looked up before.");
  }
}

const std::pair<std::size_t, std::size_t>
WorldInformationWrapper::get_edge_anchors(std::size_t id) {
  // check the given id
  check_mapping(edge_identifiers_, id);
  // look up edge and robot IDs
  boost::shared_ptr<const Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  std::pair<std::size_t, std::size_t> robot_pair(edge->getRobot1()->id(), edge->getRobot2()->id());
  // update mapping
  robot_identifiers_[edge->getRobot1()->id()] = edge->getRobot1();
  robot_identifiers_[edge->getRobot2()->id()] = edge->getRobot2();
  // return STL pair
  return robot_pair;
}

const MarkerInformationWrapper
WorldInformationWrapper::get_edge_information(std::size_t id) {
  // check the given id
  check_mapping(edge_identifiers_, id);
  // look up edge
  boost::shared_ptr<const Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  // create and return new MarkerInforamtionWrapper object
  MarkerInformationWrapper marker_information(edge->marker_information());
  return marker_information;
}

const std::vector < std::size_t >
WorldInformationWrapper::get_edges(const std::string &filter) {
  // store results in separate vector, use STL iterator
  std::vector<std::size_t> result;
  std::map< std::size_t, boost::shared_ptr<Edge> >::iterator it;
  // iterator over ALL edges (filter on the run) 
  for(it = world_information_->edges().begin(); 
      it != world_information_->edges().end(); 
      it++) {
    // get integer ID
    std::size_t id = it->second->id()->id();
    // apply filter
    std::string filter_lower(filter);
    boost::to_lower(filter_lower);
    if ((filter_lower == "directed" && !boost::dynamic_pointer_cast<DirectedEdge>(it->second)) 
      || (filter_lower == "undirected" && !boost::dynamic_pointer_cast<UndirectedEdge>(it->second))) {
      // element was filtered out
      continue;
    }
    // update mapping and store integer id in result vector
    edge_identifiers_[id] = boost::dynamic_pointer_cast<EdgeIdentifier>(it->second->id());
    result.push_back(id);
  }
  return result;
}

const std::vector<std::size_t> 
WorldInformationWrapper::get_edges(std::size_t id, const std::string &filter) {
  // check given ID
  check_mapping(robot_identifiers_, id);
  // create separate vector for results, look up corresponsing robot
  std::vector<std::size_t> result;
  boost::shared_ptr<const RobotData> robot;
  robot = world_information_->get_according_robot_data_ptr(robot_identifiers_[id]);
  // add integer representations of edge IDs to result vector, update mapping in the process
  BOOST_FOREACH (boost::shared_ptr<EdgeIdentifier> edge_identifier, robot->get_edges()) {
    std::size_t edge_id = edge_identifier->id();
    // apply filter
    std::string filter_lower(filter);
    boost::to_lower(filter_lower);
    boost::shared_ptr<Edge> edge = world_information_->get_according_edge(edge_identifier);
    if ((filter_lower == "directed" && !boost::dynamic_pointer_cast<DirectedEdge>(edge)) 
      || (filter_lower == "undirected" && !boost::dynamic_pointer_cast<UndirectedEdge>(edge))) {
      // element was filtered out
      continue;
    }
    // add ID to result vector and update mapping
    edge_identifiers_[edge_id] = edge_identifier;
    result.push_back(edge_id);
  }
  return result;
}

std::size_t
WorldInformationWrapper::get_message_id(std::size_t id) {
  // check the given ID
  check_mapping(robot_identifiers_, id);
  // look up robot and return ID of first message
  boost::shared_ptr<RobotData> rd = world_information_->get_according_robot_data_ptr(robot_identifiers_[id]);
  boost::shared_ptr<Message> msg = rd->get_message();
  return msg->id()->id();
}

std::size_t
WorldInformationWrapper::get_number_of_messages(std::size_t id) {
  // check the given ID
  check_mapping(robot_identifiers_, id);
  // look up robot and return number of messages
  boost::shared_ptr<RobotData> rd = world_information_->get_according_robot_data_ptr(robot_identifiers_[id]); 
  return rd->get_number_of_messages();
}

std::set< boost::shared_ptr<Request> >
WorldInformationWrapper::get_request_set() {
  return request_set_;
}

const MarkerInformationWrapper
WorldInformationWrapper::get_robot_information(std::size_t id) {
  // check the given ID
  check_mapping(robot_identifiers_, id);
  // look up robot
  boost::shared_ptr<const RobotData> rd = world_information_->get_according_robot_data_ptr(robot_identifiers_[id]);
  // create and return new MarkerInforamtionWrapper object
  MarkerInformationWrapper marker_information(rd->marker_information());
  return marker_information;
}

const std::vector < std::size_t > 
WorldInformationWrapper::get_robots () {
  // use separate vector to store results
  std::vector<std::size_t> result;
  // add integer representations of IDs to result vector, update mapping in the process
  BOOST_FOREACH (boost::shared_ptr<RobotData> robot, world_information_->robot_data()) {
    std::size_t id = robot->id()->id();
    robot_identifiers_[id] = boost::dynamic_pointer_cast<RobotIdentifier>(robot->id());
    result.push_back(id);
  }
  return result;
}

int
WorldInformationWrapper::get_time ()
{
  return world_information_->time();
}

void
WorldInformationWrapper::remove_edge(std::size_t id) {
  // check the given ID
  check_mapping(edge_identifiers_, id);
  // look up edge object
  boost::shared_ptr<Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  // remove map entry and edge
  edge_identifiers_.erase(id);
  world_information_->remove_edge(edge);
}

void 
WorldInformationWrapper::set_edge_information(std::size_t id, MarkerInformationWrapper marker) {
  // check the given ID
  check_mapping(edge_identifiers_, id);
  // get correct robot and create MarkerInformation object
  boost::shared_ptr<Edge> eid = world_information_->get_according_edge(edge_identifiers_[id]);
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  // set new MarkerInforamtion
  eid->set_marker_information(new_marker);
}

void 
WorldInformationWrapper::set_robot_information(std::size_t id, MarkerInformationWrapper marker) {
  // check the given ID
  check_mapping(robot_identifiers_, id);
  // get correct robot and create MarkerInformation object
  boost::shared_ptr<RobotData> rid = world_information_->get_according_robot_data_ptr(robot_identifiers_[id]);
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  // set new MarkerInforamtion
  rid->set_marker_information(new_marker);
}

void
WorldInformationWrapper::set_world_information (const boost::shared_ptr <WorldInformation> &world_information) {
  // clear mappings
  edge_identifiers_.clear();
  message_identifiers_.clear();
  robot_identifiers_.clear();
  request_set_.clear();
  // store pointer to new WorldInformation object
  world_information_ = world_information;
}
  
}


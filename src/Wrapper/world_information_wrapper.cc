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

std::size_t
WorldInformationWrapper::add_edge(std::size_t source, std::size_t target, const std::string &type) {
  MarkerInformationWrapper marker;
  return add_edge(source, target, marker, type);
} 

std::size_t
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
  // get, map and return ID of new edge
  std::size_t id = edge->id()->id();
  edge_identifiers_[id] = boost::dynamic_pointer_cast<EdgeIdentifier>(edge->id());
  return id;
}

std::size_t
WorldInformationWrapper::add_message(std::size_t sender, std::size_t receiver, MarkerInformationWrapper marker) {
  // check the given IDs
  check_mapping(robot_identifiers_, sender);
  check_mapping(robot_identifiers_, receiver);
  // create message
  boost::shared_ptr<Message> message(new Message(robot_identifiers_[sender], robot_identifiers_[receiver]));
  // create and assign MarkerInforamtion object
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  message->set_marker_information(new_marker);
  // add message
  world_information_->add_message(message);
  // get, map and return ID od new message
  std::size_t id = message->id()->id();
  message_identifiers_[id] = boost::dynamic_pointer_cast<MessageIdentifier>(message->id());
  return id;
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
    // store integer id in result vector
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
    // add ID to result vector
    result.push_back(edge_id);
  }
  return result;
}

std::size_t
WorldInformationWrapper::get_head(std::size_t id) {
  // check the given id
  check_mapping(edge_identifiers_, id);
  // look up edge and robot IDs
  boost::shared_ptr<const Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  return edge->getRobot2()->id();
}

const MarkerInformationWrapper
WorldInformationWrapper::get_message_information(std::size_t id) {
  // check the given ID
  check_mapping(message_identifiers_, id);
  // look up message
  boost::shared_ptr<Message> message = world_information_->get_according_message(message_identifiers_[id]);
  // create and return new MarkerInforamtionWrapper object
  MarkerInformationWrapper marker_information(message->marker_information());
  return marker_information;
}

const std::vector<std::size_t>
WorldInformationWrapper::get_messages() {
  // store IDs in separate result vector
  std::vector<std::size_t> result;
  // copy keys of local mapping (we do not need the message objects at this
  // point, thus we can safe some calls)
  std::map< std::size_t, boost::shared_ptr<MessageIdentifier> >::const_iterator it;
  for (it = message_identifiers_.begin();
       it != message_identifiers_.end();
       it++) {
    result.push_back(it->first);
  }
  // return result vector
  return result;
}

const std::vector<std::size_t>
WorldInformationWrapper::get_messages(std::size_t id) {
  // check the given ID
  check_mapping(robot_identifiers_, id);
  // store IDs in separate result vector
  std::vector<std::size_t> result;
  // copy keys of local mapping (we do not need the message objects at this
  // point, thus we can safe some calls)
  std::map< std::size_t, boost::shared_ptr<Message> >::const_iterator it;
  for (it = world_information_->messages().begin();
       it !=  world_information_->messages().end();
       it++) {
    // check whether message belongs to given robot
    if (it->second->receiver()->id() != id) {
      continue;
    }
    result.push_back(it->first);
  }
  return result;
}

std::size_t
WorldInformationWrapper::get_receiver(std::size_t id) {
  // check the given ID
  check_mapping(message_identifiers_, id);
  // look up message
  boost::shared_ptr<Message> message;
  message = world_information_->get_according_message(message_identifiers_[id]);
  return message->receiver()->id();
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
    result.push_back(id);
  }
  return result;
}

std::size_t
WorldInformationWrapper::get_sender(std::size_t id) {
  // check the given ID
  check_mapping(message_identifiers_, id);
  // look up message
  boost::shared_ptr<Message> message;
  message = world_information_->get_according_message(message_identifiers_[id]);
  return message->sender()->id();
}

std::size_t
WorldInformationWrapper::get_tail(std::size_t id) {
  // check the given id
  check_mapping(edge_identifiers_, id);
  // look up edge and robot IDs
  boost::shared_ptr<const Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  return edge->getRobot1()->id();
}

int
WorldInformationWrapper::get_time ()
{
  return world_information_->time();
}

bool
WorldInformationWrapper::is_directed(std::size_t id) {
  // check the given ID
  check_mapping(edge_identifiers_, id);
  // look up edge and try cast
  boost::shared_ptr<Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  return boost::dynamic_pointer_cast<DirectedEdge>(edge);
}

bool
WorldInformationWrapper::is_undirected(std::size_t id) {
  return !is_directed(id);
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
WorldInformationWrapper::remove_message(std::size_t id) {
  // check the given ID
  check_mapping(message_identifiers_, id);
  // look up message object
  boost::shared_ptr<Message> message = world_information_->get_according_message(message_identifiers_[id]);
  // remove map entry and edge
  message_identifiers_.erase(id);
  world_information_->remove_message(message);
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
  // store pointer to new WorldInformation object
  world_information_ = world_information;
  // update/clear mappings
  update_mappings();
  request_set_.clear();
}

void
WorldInformationWrapper::update_mappings() {
  // clear existing mappings
  edge_identifiers_.clear();
  message_identifiers_.clear();
  robot_identifiers_.clear();
  // look up edges
  std::map< std::size_t, boost::shared_ptr<Edge> >::iterator eit;
  for(eit = world_information_->edges().begin(); 
      eit != world_information_->edges().end(); 
      eit++) {
    std::size_t id = eit->second->id()->id();
    edge_identifiers_[id] = boost::dynamic_pointer_cast<EdgeIdentifier>(eit->second->id());
  }
  // look up messages
  std::map< std::size_t, boost::shared_ptr<Message> >::iterator mit;
  for(mit = world_information_->messages().begin(); 
      mit != world_information_->messages().end(); 
      mit++) {
    std::size_t id = mit->second->id()->id();
    message_identifiers_[id] = boost::dynamic_pointer_cast<MessageIdentifier>(mit->second->id());
  }
  // look up robots
  BOOST_FOREACH(boost::shared_ptr<RobotData> robot, world_information_->robot_data()) {
    std::size_t id = robot->id()->id();
    robot_identifiers_[id] = boost::dynamic_pointer_cast<RobotIdentifier>(robot->id());
  }
}
  
}


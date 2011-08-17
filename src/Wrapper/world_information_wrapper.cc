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

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/graph_concepts.hpp>

namespace LuaWrapper {
 
WorldInformationWrapper::WorldInformationWrapper ()
{
  /* nothing here yet */
}

// initialization of static members
boost::shared_ptr<WorldInformation> WorldInformationWrapper::world_information_;
std::set< boost::shared_ptr<Request> > WorldInformationWrapper::request_set_;
std::map< std::size_t, boost::shared_ptr<EdgeIdentifier> > WorldInformationWrapper::edge_identifiers_;
std::map< std::size_t, boost::shared_ptr<MarkerIdentifier> > WorldInformationWrapper::marker_identifiers_; 
std::map< std::size_t, boost::shared_ptr<RobotIdentifier> > WorldInformationWrapper::robot_identifiers_; 

WorldInformationWrapper::~WorldInformationWrapper ()
{

}

void
WorldInformationWrapper::set_world_information (const boost::shared_ptr <
						WorldInformation >
						&world_information)
{
  marker_identifiers_.clear();
  robot_identifiers_.clear();
  request_set_.clear();
  world_information_ = world_information;
}

std::set< boost::shared_ptr<Request> >
WorldInformationWrapper::get_request_set() {
  return request_set_;
}

void
WorldInformationWrapper::add_edge(std::size_t source, std::size_t target, const std::string &type) {
  MarkerInformationWrapper marker;
  add_edge(source, target, marker, type);
} 

void
WorldInformationWrapper::add_edge(std::size_t source, std::size_t target,
				  MarkerInformationWrapper marker, const std::string &type) {
  std::map< std::size_t, boost::shared_ptr<RobotIdentifier> >::iterator it;
  // check for source identifier
  it = robot_identifiers_.find(source);
  if (it == robot_identifiers_.end()) return;
  boost::shared_ptr<RobotIdentifier> source_robot = it->second;  
  // check for target identifier
  it = robot_identifiers_.find(target);
  if (it == robot_identifiers_.end()) return;
  boost::shared_ptr<RobotIdentifier> target_robot = it->second;  
  // create new edge
  std::string t(type);
  boost::to_lower(t);
  boost::shared_ptr<Edge> edge;
  if (t == "directed") {
    edge.reset(new DirectedEdge(source_robot, target_robot));
  } else {
    edge.reset(new UndirectedEdge(source_robot, target_robot));
  }
  // TODO: The following line copies alot of data back and forth...
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  edge->set_marker_information(new_marker);
  // add edge
  world_information_->add_edge(edge);
}  

void WorldInformationWrapper::add_marker_request(std::size_t id, MarkerInformationWrapper marker) {
  // get identifier
  std::map< std::size_t, boost::shared_ptr<RobotIdentifier> >::iterator it;
  it = robot_identifiers_.find(id);
  // if not existant, stop right here
  if (it == robot_identifiers_.end()) return;
  // get correct robot
  RobotData &rd = world_information_->get_according_robot_data(it->second);
  // create MarkerRequest
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  // TODO: Remove this evil shit (const_cast)!
  boost::shared_ptr<Request> request(new MarkerRequest(const_cast<Robot&>(rd.robot()), new_marker));
  request_set_.insert(request);
}

const std::vector<std::size_t> 
WorldInformationWrapper::get_adjacent_edges(std::size_t id) {
  std::vector<std::size_t> result;
  boost::shared_ptr<const RobotData> robot;
  
  robot = world_information_->get_according_robot_data_ptr(robot_identifiers_[id]);
  
  BOOST_FOREACH (boost::shared_ptr<EdgeIdentifier> edge_identifier, robot->get_edges()) {
    std::size_t edge_id = edge_identifier->id();
    edge_identifiers_[edge_id] = edge_identifier;
    result.push_back(edge_id);
  }

  return result;
}

const std::pair<std::size_t, std::size_t>
WorldInformationWrapper::get_edge_anchors(std::size_t id) {
  // TODO: Map access should be checked! And pointer handling (looks wrong) ...
  boost::shared_ptr<const Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  // get robot IDs and add/update map
  std::pair<std::size_t, std::size_t> robot_pair(edge->getRobot1()->id(), edge->getRobot2()->id());
  robot_identifiers_[edge->getRobot1()->id()] = edge->getRobot1();
  robot_identifiers_[edge->getRobot2()->id()] = edge->getRobot2();
  // return STL pair
  return robot_pair;
}

const MarkerInformationWrapper
WorldInformationWrapper::get_edge_information(std::size_t id) {
  // TODO: Map access should be checked! And pointer handling (looks wrong) ...
  boost::shared_ptr<const Edge> edge = world_information_->get_according_edge(edge_identifiers_[id]);
  MarkerInformationWrapper marker_information(edge->marker_information());
  return marker_information;
}

const std::vector < std::size_t >
WorldInformationWrapper::get_edges(const std::string &filter) {
  std::vector<std::size_t> result;
  
  std::map< std::size_t, boost::shared_ptr<Edge> >::iterator it;
  
  
 
  for(it = world_information_->edges().begin(); 
      it != world_information_->edges().end(); 
      it++) {
    std::size_t id = it->second->id()->id();
  
    // apply filter
    std::string f(filter);
    boost::to_lower(f);
    if ((f == "directed" && !boost::dynamic_pointer_cast<DirectedEdge>(it->second)) 
      || (f == "undirected" && !boost::dynamic_pointer_cast<UndirectedEdge>(it->second))) {
      continue;
    }
  
    edge_identifiers_[id] = boost::dynamic_pointer_cast<EdgeIdentifier>(it->second->id());
    result.push_back(id);
  }
  
  return result;
}

const MarkerInformationWrapper
WorldInformationWrapper::get_marker_information(std::size_t id) {
  // TODO: Map access should be checked! And pointer handling (looks wrong) ...
  WorldObject &marker = world_information_->get_according_marker(*(marker_identifiers_[id].get()));
  MarkerInformationWrapper marker_information(marker.marker_information());
  return marker_information;
}

const std::vector < std::size_t >
WorldInformationWrapper::get_markers () {
  std::vector<std::size_t> result;
  
  BOOST_FOREACH (boost::shared_ptr<WorldObject> marker, world_information_->markers()) {
    // the real id is visible since WorldInformationWrapper is a friend class of Identifier
    std::size_t id = marker->id()->id();
    marker_identifiers_[id] = boost::dynamic_pointer_cast<MarkerIdentifier>(marker->id());
    result.push_back(id);
  }
  
  return result;
}

const MarkerInformationWrapper
WorldInformationWrapper::get_robot_information(std::size_t id) {
  // TODO: Map access should be checked!
  RobotData &robot = world_information_->get_according_robot_data(robot_identifiers_[id]);
  MarkerInformationWrapper marker_information(robot.marker_information());
  return marker_information;
}

const std::vector < std::size_t > 
WorldInformationWrapper::get_robots () {
  std::vector<std::size_t> result;
  
  BOOST_FOREACH (boost::shared_ptr<RobotData> robot, world_information_->robot_data()) {
    // the real id is visible since WorldInformationWrapper is a friend class of Identifier
    std::size_t id = robot->id()->id();
    // TODO: Handle possible exceptions!
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
  // get identifier
  std::map< std::size_t, boost::shared_ptr<EdgeIdentifier> >::iterator it;
  it = edge_identifiers_.find(id);
  // if not existant, stop right here
  if (it == edge_identifiers_.end()) return;
  // get corresponding edge object
  boost::shared_ptr<Edge> edge = world_information_->get_according_edge(it->second);
  // remove map entry and edge
  edge_identifiers_.erase(it);
  world_information_->remove_edge(edge);
}

void 
WorldInformationWrapper::set_edge_information(std::size_t id, MarkerInformationWrapper marker) {
  // get identifier
  std::map< std::size_t, boost::shared_ptr<EdgeIdentifier> >::iterator it;
  it = edge_identifiers_.find(id);
  // if not existant, stop right here
  if (it == edge_identifiers_.end()) return;
  // get correct robot and create MarkerInformation object
  boost::shared_ptr<Edge> eid = world_information_->get_according_edge(it->second);
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  // set new MarkerInforamtion
  eid->set_marker_information(new_marker);
}

void 
WorldInformationWrapper::set_robot_information(std::size_t id, MarkerInformationWrapper marker) {
  // get identifier
  std::map< std::size_t, boost::shared_ptr<RobotIdentifier> >::iterator it;
  it = robot_identifiers_.find(id);
  // if not existant, stop right here
  if (it == robot_identifiers_.end()) return;
  // get correct robot and create MarkerInformation object
  boost::shared_ptr<RobotData> rid = world_information_->get_according_robot_data_ptr(it->second);
  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
  // set new MarkerInforamtion
  rid->set_marker_information(new_marker);
}
  
}


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

#include <boost/foreach.hpp>

namespace LuaWrapper {
 
WorldInformationWrapper::WorldInformationWrapper ()
{
  /* nothing here yet */
}

// initialization of static members
boost::shared_ptr<WorldInformation> WorldInformationWrapper::world_information_;
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
  world_information_ = world_information;
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
  
}


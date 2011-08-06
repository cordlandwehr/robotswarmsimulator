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

#include <boost/foreach.hpp>

namespace LuaWrapper {
 
WorldInformationWrapper::WorldInformationWrapper ()
{
  /* nothing here yet */
}

// initialization of static members
boost::shared_ptr<WorldInformation> WorldInformationWrapper::world_information_;
std::map< size_t, boost::shared_ptr<Identifier> > WorldInformationWrapper::marker_identifiers_; 
std::map< size_t, boost::shared_ptr<Identifier> > WorldInformationWrapper::robot_identifiers_; 

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

const std::vector < size_t >
WorldInformationWrapper::get_markers () {
  std::vector<size_t> result;
  
  BOOST_FOREACH (boost::shared_ptr<WorldObject> marker, world_information_->markers()) {
    // the real id is visible since WorldInformationWrapper is a friend class of Identifier
    size_t id = marker->id()->id();
    marker_identifiers_[id] = marker->id();
    result.push_back(id);
  }
  
  return result;
}

const std::vector < size_t > 
WorldInformationWrapper::get_robots () {
  std::vector<size_t> result;
  
  BOOST_FOREACH (boost::shared_ptr<RobotData> robot, world_information_->robot_data()) {
    // the real id is visible since WorldInformationWrapper is a friend class of Identifier
    size_t id = robot->id()->id();
    robot_identifiers_[id] = robot->id();
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


//
//  world_information_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//


#ifndef WORLD_INFORMATION_WRAPPER_H
#define WORLD_INFORMATION_WRAPPER_H

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"

namespace LuaWrapper {
 
class WorldInformationWrapper
{

public:
  WorldInformationWrapper ();
  virtual ~ WorldInformationWrapper ();
  
  static void set_world_information(const boost::shared_ptr<WorldInformation> &world_information);
  
  // actual wrapper methods
  static const std::vector<size_t> get_markers();
  static const std::vector<size_t> get_robots();
  static int get_time();
  
private:
  static boost::shared_ptr<WorldInformation> world_information_;
  static std::map< size_t, boost::shared_ptr<Identifier> > marker_identifiers_;
  static std::map< size_t, boost::shared_ptr<Identifier> > robot_identifiers_;
};
  
}

#endif // WORLD_INFORMATION_WRAPPER_H

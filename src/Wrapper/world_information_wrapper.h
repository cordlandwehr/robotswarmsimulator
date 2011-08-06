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
#include "../Wrapper/marker_information_wrapper.h"

namespace LuaWrapper {
 
class WorldInformationWrapper
{

public:
  WorldInformationWrapper ();
  virtual ~ WorldInformationWrapper ();
  
  static void set_world_information(const boost::shared_ptr<WorldInformation> &world_information);
  
  // actual wrapper methods
  static const MarkerInformationWrapper get_marker_information(std::size_t id);
  static const std::vector<std::size_t> get_markers();
  static const MarkerInformationWrapper get_robot_information(std::size_t id);
  static const std::vector<std::size_t> get_robots();
  static int get_time();
  
private:
  static boost::shared_ptr<WorldInformation> world_information_;
  static std::map< std::size_t, boost::shared_ptr<MarkerIdentifier> > marker_identifiers_;
  static std::map< std::size_t, boost::shared_ptr<RobotIdentifier> > robot_identifiers_;
};
  
}

#endif // WORLD_INFORMATION_WRAPPER_H

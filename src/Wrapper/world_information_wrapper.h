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
#include <set>
#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Requests/request.h"
#include "../Wrapper/marker_information_wrapper.h"

namespace LuaWrapper {
 
class WorldInformationWrapper
{

public:
  WorldInformationWrapper ();
  virtual ~ WorldInformationWrapper ();
  
  static void set_world_information(const boost::shared_ptr<WorldInformation> &world_information);
  static std::set< boost::shared_ptr<Request> > get_request_set();
  
  // actual wrapper methods
  static void add_marker_request(std::size_t id, MarkerInformationWrapper marker);
  static const MarkerInformationWrapper get_marker_information(std::size_t id);
  static const std::vector<std::size_t> get_markers();
  static const MarkerInformationWrapper get_robot_information(std::size_t id);
  static const std::vector<std::size_t> get_robots();
  static int get_time();
  
private:
  static boost::shared_ptr<WorldInformation> world_information_;
  static std::set< boost::shared_ptr<Request> > request_set_;
  static std::map< std::size_t, boost::shared_ptr<MarkerIdentifier> > marker_identifiers_;
  static std::map< std::size_t, boost::shared_ptr<RobotIdentifier> > robot_identifiers_;
};
  
}

#endif // WORLD_INFORMATION_WRAPPER_H

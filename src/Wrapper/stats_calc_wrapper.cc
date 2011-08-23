//
//  stats_calc_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"
#include "../Statistics/stats_calc.h"
#include "../Wrapper/world_information_wrapper.h"
#include "../Wrapper/stats_calc_wrapper.h"

namespace LuaWrapper {
  
StatsCalcWrapper::StatsCalcWrapper () {}

StatsCalcWrapper::~StatsCalcWrapper () {}

int
StatsCalcWrapper::calculate_degree(const std::vector<std::size_t>& ids) {
  // check the given IDs
  BOOST_FOREACH(std::size_t id, ids) {
    WI::check_mapping(WI::robot_identifiers_, id);
  }
  // store RobotData objects in new vector
  std::vector< boost::shared_ptr<RobotData> > robots;
  BOOST_FOREACH(std::size_t id, ids) {
    boost::shared_ptr<RobotIdentifier> robot_id = WI::robot_identifiers_[id];
    robots.push_back(WI::world_information_->get_according_robot_data_ptr(robot_id));
  }
  // call StatsCalc method and return result
  return StatsCalc::calculate_degree(WI::world_information_);
}

int
StatsCalcWrapper::calculate_maximal_defect() {
  return StatsCalc::calculate_maximal_defect(WI::world_information_);
}

int
StatsCalcWrapper::calculate_total_defects() {
  return StatsCalc::calculate_total_defects(WI::world_information_);
}
  
}

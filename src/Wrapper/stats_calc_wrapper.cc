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
StatsCalcWrapper::calculate_degree() {
  return StatsCalc::calculate_degree(WI::world_information_);
}

int 
StatsCalcWrapper::calculate_hop_distance(std::size_t start, std::size_t target, std::vector<std::size_t> ignore) {
  // check start ad target IDs
  WI::check_mapping(WI::robot_identifiers_, start);
  WI::check_mapping(WI::robot_identifiers_, target);
  // check ever edge to be ignored and create EdgeIdentifier vector on the fly
  std::vector< boost::shared_ptr<EdgeIdentifier> > ignore_ptrs;
  BOOST_FOREACH(std::size_t edge_id, ignore) {
    WI::check_mapping(WI::edge_identifiers_, edge_id);
    ignore_ptrs.push_back(WI::edge_identifiers_[edge_id]);
  }
  // call StatsCalc method
  return StatsCalc::calculate_hop_distance(WI::world_information_, WI::robot_identifiers_[start], WI::robot_identifiers_[target], ignore_ptrs);
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

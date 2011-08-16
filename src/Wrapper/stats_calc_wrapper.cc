//
//  stats_calc_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//

#include "stats_calc_wrapper.h"

namespace LuaWrapper {
  
StatsCalcWrapper::StatsCalcWrapper ()
{

}

StatsCalcWrapper::~StatsCalcWrapper ()
{

}

// initialization of static members
boost::shared_ptr<StatsCalc> StatsCalcWrapper::stats_calc_;

void 
StatsCalcWrapper::set_stats_calc (const boost::shared_ptr <StatsCalc> stats_calc) {
  stats_calc_ = stats_calc;
}
  
}
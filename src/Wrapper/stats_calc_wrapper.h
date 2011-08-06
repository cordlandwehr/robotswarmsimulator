//
//  stats_calc_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//

#ifndef STATS_CALC_WRAPPER_H
#define STATS_CALC_WRAPPER_H

#include <boost/shared_ptr.hpp>

#include "../Statistics/stats_calc.h"

namespace LuaWrapper {

class StatsCalcWrapper
{

public:
  StatsCalcWrapper ();
  virtual ~ StatsCalcWrapper ();
  
  static void set_stats_calc(const boost::shared_ptr<StatsCalc> stats_calc);
  
private:
  static boost::shared_ptr<StatsCalc> stats_calc_;
};
 
}

#endif // STATS_CALC_WRAPPER_H

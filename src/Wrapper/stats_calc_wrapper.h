//
//  stats_calc_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//

#ifndef STATS_CALC_WRAPPER_H
#define STATS_CALC_WRAPPER_H

#include <vector>

namespace LuaWrapper {

class StatsCalcWrapper
{

public:
  StatsCalcWrapper ();
  virtual ~ StatsCalcWrapper ();
  
  /**
   * Calculates maximal degree for the given set of robots.
   * 
   * \param ids		IDs of the robots.
   * \returns		Maximal degree.
   */
  static int calculate_degree(const std::vector<std::size_t>& ids);
  
  /**
   * Calculates maximal defect.
   * 
   * \returns 		Maximal defect.
   */
  static int calculate_maximal_defect();
  
  /**
   * Calculates total defects.
   * 
   * \returns 		Total defects.
   */
  static int calculate_total_defects();
  
private:
  typedef WorldInformationWrapper WI;
};
 
}

#endif // STATS_CALC_WRAPPER_H

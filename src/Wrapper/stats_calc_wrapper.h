//
//  stats_calc_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//

#ifndef STATS_CALC_WRAPPER_H
#define STATS_CALC_WRAPPER_H

#include <vector>

#include "../Wrapper/world_information_wrapper.h"

namespace LuaWrapper {

class StatsCalcWrapper
{

public:
  StatsCalcWrapper ();
  virtual ~ StatsCalcWrapper ();
  
  /**
   * Calculates maximal degree of the graph.
   * 
   * \returns		Maximal degree.
   */
  static int calculate_degree();
  
  /**
   * Calculates hop distance between a pair of robots.
   * 
   * \param start	Start point (robot).
   * \param target	End point (robot).
   * \param ignore	List of edges to Ignore.
   * \returns		Hop distance.
   */
  static int calculate_hop_distance(std::size_t start, std::size_t target, std::vector<std::size_t> ignore);
  
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

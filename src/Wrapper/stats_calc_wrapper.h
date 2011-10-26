//
//  stats_calc_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//

#ifndef STATS_CALC_WRAPPER_H
#define STATS_CALC_WRAPPER_H

#include <vector>
#include <string>

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
   * Calculates diameter of the graph but checks for the direction of the long-range links.
   *   
   * \returns		Diameter.
   */
  static int calculate_diameter_with_long_range();  
  
  /**
   * Calculates maximal defect.
   * 
   * \returns 		Maximal defect.
   */
  static int calculate_maximal_defect(const std::string& role);
  static int calculate_maximal_defect_default();

  /**
   * Calculates total defects.
   * 
   * \returns 		Total defects.
   */
  static int calculate_total_defects();
  
  /**
   * Calculates distribution of hop distances (using a local greedy strategy) for the
   * the first Move and Forget experiment.
   * 
   * \param graph		Current WorldInformation.
   * \returns		A vector of doubles. Values: (Min, q.125, q.25, q.375, Median,
   *			q.625, q.750, q.875, Max, Mean, SD, Skewness, Exess).
   */
  static const std::vector<double> evaluate_first_mf_experiment();
  
  /**
	 * Calculates possible advantages of long range links per hop for local greedy routing
	 * in a circle.
	 * 
	 * \param graph 	Currennt WorldInformation.
	 * \returns		A vector of pairs of mean and SD values beginning with distance
	 * 			two. 
	 **/
  static const std::vector<double> evaluate_uniform_lrlink_distribution();

  /**
   * Evaluates a polynomial at a given value.
   *
   * \note This is a wrapper of boost::math::tools::evaluate_polynomial
   *
   * \param poly The polynomial given as an vector of coefficients.
   * \param val The value to evaluate against.
   * \return The evaluated polynom.
   */
  static double evaluate_polynomial(std::vector<double> poly, double val);

  /**
   * Generates a list of prime numbers in the given interval.
   * The 'Sieve of Eratosthenes' algorithm is used to generate the primes.
   *
   * \note The source code was copied from here 'http://code.activestate.com/recipes/576559-fast-prime-generator/'
   *
   * \param min Start of the interval.
   * \param max End of the interval.
   * \return A list of prime numbers.
   */
  static std::vector<unsigned long> generate_primes(unsigned long min, unsigned long max);

private:
  typedef WorldInformationWrapper WI;
};
 
}

#endif // STATS_CALC_WRAPPER_H

//
//  stats_calc_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//

#include <iostream>
#include <vector>
#include <string.h>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/math/tools/rational.hpp>

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
 StatsCalcWrapper::calculate_diameter_with_long_range() {
   
   //not using ingnore_ptrs atm
  std::vector< boost::shared_ptr<EdgeIdentifier> > ignore_ptrs;

  return StatsCalc::calculate_diameter_and_ignore_long_range_links_in_one_dir(WI::world_information_, ignore_ptrs);   
   
}


int
StatsCalcWrapper::calculate_maximal_defect() {
  return StatsCalc::calculate_maximal_defect(WI::world_information_);
}

int
StatsCalcWrapper::calculate_total_defects() {
  return StatsCalc::calculate_total_defects(WI::world_information_);
}

const std::vector<double>
StatsCalcWrapper::evaluate_first_mf_experiment() {
  return StatsCalc::evaluate_first_mf_experiment(WI::world_information_);
}
  
double StatsCalcWrapper::evaluate_polynomial(std::vector<double> poly, double val) {
	return boost::math::tools::evaluate_polynomial(poly.data(), val, poly.size());
}

std::vector<unsigned long> StatsCalcWrapper::generate_primes(unsigned long min, unsigned long max) {
	std::vector<unsigned long> primes;
	char *sieve;
	sieve = new char[max/8+1];
	// Fill sieve with 1
	memset(sieve, 0xFF, (max/8+1) * sizeof(char));
	for(unsigned long x = 2; x <= max; x++)
		if(sieve[x/8] & (0x01 << (x % 8))){
			if(x>=min)
				primes.push_back(x);
			// Is prime. Mark multiplicates.
			for(unsigned long j = 2*x; j <= max; j += x)
				sieve[j/8] &= ~(0x01 << (j % 8));
		}
	delete[] sieve;
	return primes;
}

}

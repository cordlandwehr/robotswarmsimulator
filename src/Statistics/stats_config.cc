/*
 * stats_config.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include <string>

#include "stats_config.h"
#include "vecset_stats.h"

StatsConfig::StatsConfig() {
}

StatsConfig::~StatsConfig() {
}

/*
 *
 *
 * Initialization functions following...
 *
 *
 */

void StatsConfig::init(boost::shared_ptr<Parser> parser) {

	// get subsets-configuration from Parser's STATS_SUBSETS = ...
	// e.g. STATS_SUBSETS = {ALL} {MASTERS} {INACTALL} ...
	std::string s = parser->statistics_subsets();

	// initialize subsets-configuration
	subset_all_          = (s.find("{ALL}", 0) != std::string::npos);
	subset_actall_       = (s.find("{ACTALL}", 0) != std::string::npos);
	subset_inactall_     = (s.find("{INACTALL}", 0) != std::string::npos);
	subset_masters_      = (s.find("{MASTERS}", 0) != std::string::npos);
	subset_actmasters_   = (s.find("{ACTMASTERS}", 0) != std::string::npos);
	subset_inactmasters_ = (s.find("{INACTMASTERS}", 0) != std::string::npos);
	subset_slaves_       = (s.find("{SLAVES}", 0) != std::string::npos);
	subset_actslaves_    = (s.find("{ACTSLAVES}", 0) != std::string::npos);
	subset_inactslaves_  = (s.find("{INACTSLAVES}", 0) != std::string::npos);

	any_subset_ = subset_all_ || subset_actall_ || subset_inactall_
				|| subset_masters_ || subset_actmasters_ || subset_inactmasters_
				|| subset_slaves_ || subset_actslaves_ || subset_inactslaves_;

	std::cout << "    statistics-subsets are: "
			  << (subset_all_ ? "ALL " : "")
			  << (subset_actall_ ? "ACTALL " : "")
			  << (subset_inactall_ ? "INACTALL " : "")
			  << (subset_masters_ ? "MASTERS " : "")
			  << (subset_actmasters_ ? "ACTMASTERS " : "")
			  << (subset_inactmasters_ ? "INACTMASTERS " : "")
			  << (subset_slaves_ ? "SLAVES " : "")
			  << (subset_actslaves_ ? "ACTSLAVES " : "")
			  << (subset_inactslaves_ ? "INACTSLAVES " : "") << std::endl;

	// get template-configuration from Parser's STATS_TEMPLATE = ...
	// e.g. STATS_TEMPLATE = DEFAULT
	s = parser->statistics_template();

	// initialize template
	std::cout << "    statistics-configuration is: ";
	if (s.find("ALL", 0) != std::string::npos) {
		init_activate_all();
		std::cout << "ALL" << std::endl;
	} else if (s.find("BASIC", 0) != std::string::npos) {
		init_activate_basic();
		std::cout << "BASIC" << std::endl;
	} else if (s.find("NONE", 0) != std::string::npos) {
		init_activate_none();
		std::cout << "NONE" << std::endl;
	} else {
		std::cerr << "invalid value for STATS_TEMPLATE in projectfile. Using STATS_TEMPLATE = NONE" << std::endl;
		init_activate_none();
	}

	// TODO get any additional configuration from Parser
	// e.g. from "name=value" - pairs in the projectfile.
	// Needs changes to Parser for accessing any field in projectfile.
}

const void StatsConfig::init_activate_all() {
	num_robots_ = num_masters_ = num_slaves_ = true;
	swarm_avg_pos_ = true;
	miniball_center_ = miniball_radius_ = miniball_movedist_ = true;
	vel_cfg_ = VecSetStats::ALL;

	// TODO assign all other values
}

const void StatsConfig::init_activate_basic() {
	num_robots_ = num_masters_ = num_slaves_ = false;
	swarm_avg_pos_ = true;
	miniball_center_ = false;
	miniball_radius_ = miniball_movedist_ = true;
	vel_cfg_ = 0;

	// TODO assign all other values
}

const void StatsConfig::init_activate_none() {
	num_robots_ = num_masters_ = num_slaves_ = false;
	swarm_avg_pos_ = false;
	miniball_center_ = miniball_radius_ = miniball_movedist_ = false;
	vel_cfg_ = 0;

	// TODO assign all other values
}

/*
 *
 *
 * Subset-functions following...
 *
 *
 */

const bool StatsConfig::is_any_subset() const {
	return any_subset_;
}

const bool StatsConfig::is_subset_all() const {
	return subset_all_;
}

const bool StatsConfig::is_subset_actall() const {
	return subset_actall_;
}

const bool StatsConfig::is_subset_inactall() const {
	return subset_inactall_;
}

const bool StatsConfig::is_subset_masters() const {
	return subset_masters_;
}

const bool StatsConfig::is_subset_actmasters() const {
	return subset_actmasters_;
}

const bool StatsConfig::is_subset_inactmasters() const {
	return subset_inactmasters_;
}

const bool StatsConfig::is_subset_slaves() const {
	return subset_slaves_;
}

const bool StatsConfig::is_subset_actslaves() const {
	return subset_actslaves_;
}

const bool StatsConfig::is_subset_inactslaves() const {
	return subset_inactslaves_;
}

/*
 *
 *
 * Calculation-parameter following...
 *
 *
 */

const bool StatsConfig::is_num_robots() const {
	return num_robots_;
}

const bool StatsConfig::is_num_masters() const {
	return num_masters_;
}

const bool StatsConfig::is_num_slaves() const {
	return num_slaves_;
}

const bool StatsConfig::is_swarm_avg_pos() const {
	return swarm_avg_pos_;
}

const bool StatsConfig::is_miniball_center() const {
	return miniball_center_;
}

const bool StatsConfig::is_miniball_radius() const {
	return miniball_radius_;
}

const bool StatsConfig::is_miniball_movedist() const {
	return miniball_movedist_;
}

const int StatsConfig::vel_cfg() const {
	return vel_cfg_;
}

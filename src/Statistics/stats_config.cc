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

void StatsConfig::init(std::map<std::string, std::string> &params) {

	// get subsets-configuration from Parser's STATS_SUBSETS = ...
	// e.g. STATS_SUBSETS = {ALL} {MASTERS} {INACTALL} ...
	std::string s = params["STATISTICS_SUBSETS"];

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

	std::string subsetString = "Subsets are: ";
	subset_all_ ? subsetString += "{ALL} " : "";
	subset_actall_ ? subsetString += "{ACTALL} " : "";
	subset_inactall_ ? subsetString += "{INACTALL} " : "";
	subset_masters_ ? subsetString += "{MASTERS} " : "";
	subset_actmasters_ ? subsetString += "{ACTMASTERS} " : "";
	subset_inactmasters_ ? subsetString += "{INACTMASTERS} " : "";
	subset_slaves_ ? subsetString += "{SLAVES} " : "";
	subset_actslaves_ ? subsetString += "{ACTSLAVES} " : "";
	subset_inactslaves_ ? subsetString += "{INACTSLAVES} " : "";

	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << subsetString;

	// get template-configuration from Parser's STATS_TEMPLATE = ...
	// e.g. STATS_TEMPLATE = DEFAULT
	s = params["STATISTICS_TEMPLATE"];

	// initialize template

	if (s.find("ALL", 0) != std::string::npos) {
		init_activate_all();
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) <<"Configuration is: ALL";
	} else if (s.find("BASIC", 0) != std::string::npos) {
		init_activate_basic();
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Configuration is: BASIC";
	} else if (s.find("NONE", 0) != std::string::npos) {
		init_activate_none();
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Configuration is: NONE";
	} else {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::error) << "Invalid value for STATS_TEMPLATE in projectfile. Using STATS_TEMPLATE = NONE";
		init_activate_none();
	}

	// get datadump-configuration from Parser's STATS_DATADUMP = ...
	// e.g. STATS_DATADUMP = FULL
	s = params["STATISTICS_DATADUMP"];
	if (s == "FULL") {
		datadump_level_ = DATADUMP_FULL;
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Datadump is: FULL";
	} else {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Datadump is: NONE";
		if (s != "" && s != "NONE")
			ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Invalid value for STATS_DATADUMP in projectfile. Using STATS_DATADUMP = NONE";

		datadump_level_ = DATADUMP_NONE;
	}

}

const void StatsConfig::init_activate_all() {
	num_robots_ = num_masters_ = num_slaves_ = true;
	swarm_avg_pos_ = true;
	miniball_center_ = miniball_radius_ = miniball_movedist_ = volume_quot_ = true;
	vel_cfg_ = VecSetStats::ALL;
	visgraph_connected_ = true;
	max_mindist_ = max_origindist_ = max_l1origindist_ = true;
}

const void StatsConfig::init_activate_basic() {
	num_robots_ = num_masters_ = num_slaves_ = false;
	swarm_avg_pos_ = true;
	miniball_center_ = false;
	miniball_radius_ = miniball_movedist_ = volume_quot_ = true;
	vel_cfg_ = 0;
	visgraph_connected_ = false;
	max_mindist_ = max_origindist_ = max_l1origindist_ = false;
}

const void StatsConfig::init_activate_none() {
	num_robots_ = num_masters_ = num_slaves_ = false;
	swarm_avg_pos_ = false;
	miniball_center_ = miniball_radius_ = miniball_movedist_ = volume_quot_ = false;
	vel_cfg_ = 0;
	visgraph_connected_ = false;
	max_mindist_ = max_origindist_ = max_l1origindist_ = false;
}

const int StatsConfig::datadump_level() const {
	return datadump_level_;
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

const bool StatsConfig::is_volume_quot() const {
	return volume_quot_;
}

const int StatsConfig::vel_cfg() const {
	return vel_cfg_;
}

const bool StatsConfig::is_visgraph_connected() const {
	return visgraph_connected_;
}

const bool StatsConfig::is_max_mindist() const {
	return max_mindist_;
}

const bool StatsConfig::is_max_origindist() const {
	return max_origindist_;
}

const bool StatsConfig::is_max_l1origindist() const {
	return max_l1origindist_;
}

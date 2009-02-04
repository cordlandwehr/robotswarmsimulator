/*
 * stats_config.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include "stats_config.h"

StatsConfig::StatsConfig() {
	// TODO Auto-generated constructor stub

}

StatsConfig::~StatsConfig() {
	// TODO Auto-generated destructor stub
}

void StatsConfig::init(Parser& parser) {
	// initialize subsets
	//TODO subset_all_ = subset_masters_ = subset_slaves_ = true;
}

const void StatsConfig::init_activate_all() {
	num_robots_ = num_masters_ = num_slaves_ = true;
	swarm_avg_pos_ = true;
}

const void StatsConfig::init_activate_basic() {
	num_robots_ = num_masters_ = num_slaves_ = false;
	swarm_avg_pos_ = true;
}

const void StatsConfig::init_activate_none() {
	num_robots_ = num_masters_ = num_slaves_ = false;
	swarm_avg_pos_ = false;
}

const bool StatsConfig::is_subset_all() const {
	return subset_all_;
}

const bool StatsConfig::is_subset_masters() const {
	return subset_masters_;
}

const bool StatsConfig::is_subset_slaves() const {
	return subset_slaves_;
}

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

const int StatsConfig::vel_cfg() const {
	return vel_cfg_;
}

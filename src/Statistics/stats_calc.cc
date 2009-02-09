/*
 * stats_calc.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include "stats_calc.h"

StatsCalc::StatsCalc() {
	// TODO Auto-generated constructor stub

}

StatsCalc::~StatsCalc() {
	// TODO Auto-generated destructor stub
}

void StatsCalc::init(StatsConfig & stats_cfg) {
	this->stats_cfg = stats_cfg;
}

void StatsCalc::calculate(StatsCalcInData & data,
		std::vector<boost::shared_ptr<RobotData> > & subset,
		boost::shared_ptr<StatsOut> & stats_out) {

	std::vector<double> values;
	std::vector<double> tmp_numstats_vector;

	std::vector<string> names;
	bool push_names = !stats_out->is_open();

	if (stats_cfg.is_num_robots()) {
		// log num_robots
		values.push_back(subset.size());
		if (push_names)
			names.push_back("num_robots");
	}

	if (stats_cfg.vel_cfg() != 0) {
		// log velocity-information
		tmp_numstats_vector.clear();
		for(unsigned int i=0; i<subset.size(); i++) {
			//"tmp_numstats_vector.push_back((*subset[i])->velocity_len());"
		}

		num_stats.handle(tmp_numstats_vector, stats_cfg.vel_cfg());
		num_stats.push_values(values);
		if (push_names)
			num_stats.push_names(names, "vel");
	}

	// DO ALL THE CALCULATION
	// ...

	if (push_names)
		(*stats_out).open(names);

	stats_out->update(data.world_info_->time(), values);
}

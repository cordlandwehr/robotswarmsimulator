/*
 * stats_calc.h
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#ifndef STATS_CALC_H_
#define STATS_CALC_H_

#include "../Model/world_information.h"
#include "../SimulationKernel/simulation_listener.h"

#include "stats_config.h"
#include "stats_calc_indata.h"
#include "numset_stats.h"
#include "vecset_stats.h"
#include "stats_out.h"

class StatsCalc {
public:
	StatsCalc();
	virtual ~StatsCalc();

	void init(StatsConfig stats_cfg);

	void calculate(StatsCalcInData data,
			std::vector<boost::shared_ptr<RobotData> > subset,
			StatsOut stats_out);

private:
	StatsConfig stats_cfg;
	NumSetStats num_stats;
	VecSetStats vec_stats;
};

#endif /* STATS_CALC_H_ */

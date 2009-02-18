/*
 * stats_calc.h
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#ifndef STATS_CALC_H_
#define STATS_CALC_H_

#include <boost/smart_ptr.hpp>

#include "../Model/world_information.h"
#include "../SimulationKernel/simulation_listener.h"

#include "stats_config.h"
#include "stats_out.h"
#include "numset_stats.h"
#include "vecset_stats.h"

struct StatsCalcInData {

	/**
	 * Holds a copy of the latest WorldInformation for the simulation-time world_info_.time().
	 * It is guranteed that *all* events for exactly that time have already been processed.
	 */
	boost::shared_ptr<WorldInformation> world_info_;

	/**
	 * After performing the calculations on world_info_ it is moved to prev_world_info_ for
	 * latter additional calculations on the differences to future world_info_.
	 * At simulation's begin it will hold that "prev_world_info_.get() == NULL"
	 */
	boost::shared_ptr<WorldInformation> prev_world_info_;

	boost::shared_ptr<std::vector<Vector3d> > prev_positions;
	Vector3d prev_miniball_center;
	double prev_miniball_radius;
};

class StatsCalc {
public:

	static const bool DEBUG = false;

	StatsCalc();
	virtual ~StatsCalc();

	void init(StatsConfig* stats_cfg);

	void calculate(StatsCalcInData &data,
			std::vector<boost::shared_ptr<RobotData> > & subset,
			boost::shared_ptr<StatsOut> & stats_out);

private:
	/**
	 * pointer to the stats_cfg stored in StatsControl
	 */
	StatsConfig* stats_cfg;
	NumSetStats num_stats;
	VecSetStats vec_stats;
};

#endif /* STATS_CALC_H_ */

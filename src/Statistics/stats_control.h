/*
 * stats_control.h
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#ifndef STATS_CONTROL_H_
#define STATS_CONTROL_H_

#include <vector>

#include "../Utilities/parser.h"

#include "stats_config.h"
#include "stats_calc.h"
#include "stats_calc_indata.h"
#include "stats_calc.h"

//class StatsControl {
//public:
//	StatsControl();
//	virtual ~StatsControl();
//
//	void init(Parser& parser);
//
//	void update(const WorldInformation& world_information,
//			    boost::shared_ptr<Event> event);
//
//	void quit();
//
//private:
//	StatsConfig stats_cfg_;
//	StatsCalc stats_calc_;
//	StatsCalcInData stats_calc_indata_;
//	std::vector<StatsOut> stats_out_;
//	bool stats_initialized_;
//
//	std::vector<std::vector<boost::shared_ptr<RobotData> > > cur_subsets_;
//
//	/**
//	 * updates all the subsets.
//	 */
//	void update_subsets();
//
//	/**
//	 * performs a calculation. That includes any update of the
//	 */
//	void calculate();
//
//
//};

#endif /* STATS_CONTROL_H_ */

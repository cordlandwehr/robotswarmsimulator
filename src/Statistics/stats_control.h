/*
 * stats_control.h
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#ifndef STATS_CONTROL_H_
#define STATS_CONTROL_H_

#include <boost/smart_ptr.hpp>

#include <vector>

#include "../Utilities/parser.h"
#include "../Model/robot_data.h"

#include "stats_config.h"
#include "stats_calc.h"
#include "stats_calc_indata.h"
#include "stats_calc.h"

class StatsControl {
public:
	StatsControl();
	virtual ~StatsControl();

	void init(Parser& parser);

	void update(const WorldInformation& world_information,
			    boost::shared_ptr<Event> event);

	void quit();

private:
	/**
	 * the configuration-instance to use
	 */
	StatsConfig stats_cfg_;

	/**
	 * the calculation-instance to use
	 */
	StatsCalc stats_calc_;

	/**
	 * the data-object used for passing data to the calculation
	 */
	StatsCalcInData stats_calc_indata_;

	/**
	 * the log-output-instances one for each subset
	 */
	std::vector<boost::shared_ptr<StatsOut> > stats_out_;

	/**
	 * all the subsets under inspection - up to 9, but only those
	 * that were requested. They appear in order:
	 *
	 * [ALL] [ACTALL] [INACTALL]
	 * [MASTERS] [ACTMASTERS] [INACTMASTERS]
	 * [SLAVES] [ACTSLAVES] [INACTSLAVES]
	 *
	 * the inner vector holds shared pointers to the robots' data of that subset
	 */
	std::vector<std::vector<boost::shared_ptr<RobotData> > > cur_subsets_;

	bool stats_initialized_;

	/**
	 * updates all the subsets in cur_subsets_
	 */
	void update_subsets();

	/**
	 * performs a calculation. That includes any update of the
	 */
	void calculate();

};

#endif /* STATS_CONTROL_H_ */

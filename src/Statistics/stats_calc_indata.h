/*
 * stats_calc_indata.h
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#ifndef STATS_CALC_INDATA_H_
#define STATS_CALC_INDATA_H_

#include "../Model/world_information.h"

struct StatsCalcInData {
	WorldInformation world_info_;
	WorldInformation prev_world_info_;
};

#endif /* STATS_CALC_INDATA_H_ */

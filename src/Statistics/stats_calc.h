/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * \class	stats_calc
 * \author	Tobias Isenberg
 * \brief	performs all the statistical calculations
 *
 * The stats_calc-class implements the pure calculation for a given worldinformation.
 * Therefore it receives from StatsControl an updated
 * struct StatsCalcInData with all information needed.
 */

// BEGIN STRUCT-DEFINTION

/**
 * StatsCalcInData
 * \author	Tobias Isenberg
 * \brief	holds information on current and last worldinformation
 *
 * This struct is filled by the StatsControl with the latest worldinformation
 * for a given point in time. Then it is sent to stats_calc as input for performing
 * all calculations. Some of them might need to access the previous worldinformation,
 * so the previous one is saved here, too. Additionally the stats_calc might perform
 * some time-consuming operations whose result shouldn't be recalculated when
 * accessing the previous worldinformation, so these results are also saved here.
 */

#ifndef STATS_CALC_H_
#define STATS_CALC_H_

#include <boost/smart_ptr.hpp>

#include "../Model/world_information.h"
#include "../SimulationKernel/simulation_listener.h"
#include "../Utilities/console_output.h"

#include "stats_out.h"
//#include "statistics_data_object.h"

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


	// SAVED PRECALCULATED VALUES
	// for all subsets in one vector.
	/*none so far*/
};

// END STRUCT-DEFINTION

class StatsCalc {
public:

	static const bool DEBUG = false;


	static int calculate_degree(const boost::shared_ptr<WorldInformation> graph);

	static int calculate_maximal_defect(const boost::shared_ptr<WorldInformation> graph);

	static int calculate_total_defects(const boost::shared_ptr<WorldInformation> graph);

	static std::size_t calculate_hop_distance(const boost::shared_ptr<WorldInformation> graph,
				 	 	 	 	 	 	 	 boost::shared_ptr<RobotIdentifier> start,
				 	 	 	 	 	 	 	 boost::shared_ptr<RobotIdentifier> target,
				 	 	 	 	 	 	 	 const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore);
private:
};

#endif /* STATS_CALC_H_ */

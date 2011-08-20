/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
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
 * \class	StatsControl
 * \author	Sven Kurras
 * \brief	external: interface to simulation, internal: managing all calculations.
 *
 * 1. external:
 * In an external view this class is the interface for the simulation to the
 * statistics-calculation. The simulation simply has to create an instance of this
 * class and init it with a parser's parametermap. Any internal needed structures
 * are handled internally and transparent to outside.
 * For receiving intermediate data this StatsControl must be registered as a
 * SimulationListener. It will automatically only perform the calcuations for the
 * latest worldinformation of a given time (for the case that multiple events at
 * the same time occur). Additionally the StatsControl's update-function should be
 * called with the initial worldinformation (with an empty event) and for a clean
 * shutdown of this module its quit()-function should be called when quitting the
 * simulation.
 * It is ok to reuse the same instance for multiple preceding simulations as long as
 * at the beginning of each simulation init() is called and at the end of
 * each simulation quit() is called (though there is an auto-quit on next call of
 * init(), but it will generate a warning at std::cerr).
 *
 * 2. internal:
 * This class creates and holds the configuration StatsConfig and all the needed
 * StatsOut (one for each subset) for logging the output and the StatsCalc for
 * the calculations itself. This class manages the input-data for transferring to
 * StatsCalc, especially collecting the incoming worldinformations as long as it
 * receives one for a true latter point in time.
 *
 * 3. datadump:
 * Additionally the datadump-functionality is implemented here.
 */

#ifndef STATS_CONTROL_H_
#define STATS_CONTROL_H_

#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "../Events/event.h"
#include "../Model/robot_data.h"
#include "../Model/world_information.h"
#include "../SimulationKernel/simulation_listener.h"
#include "../Utilities/parser.h"

#include "stats_calc.h"

class StatisticsDataObject;

class StatsControl : public SimulationListener {
public:
	static const bool DEBUG = false;

	StatsControl();
	virtual ~StatsControl();

	/**
	 * Initializes this StatsControl and creates and
	 * initializes all subcomponents configured by the
	 * given projectfile.
	 * \param parser the parser to use for configuration
	 * \param output_dir the directory for all output files, created iff does not exists
	 */
	void init(std::string output_dir);

	/**
	 * implemented from SimulationListener.
	 * \param world_information the current worldinformation. Must not be NULL.
	 * \param event the current event. Can be empty shared_ptr (event.get()==NULL)
	 */
	void update(TimePoint& time_point,
			    boost::shared_ptr<Event> event);

	/**
	 * Quits all subcomponents (especially the output-files are closed).
	 */
	void quit();

private:
	/**
	 * the configuration-instance to use
	 */
	boost::shared_ptr<StatsOut> stats_datadump_;

	/**
	 * the calculation-instance to use
	 */
	StatsCalc stats_calc_;

	/**
	 * the data-object used for passing data to the calculation
	 */
	StatsCalcInData stats_calc_indata_;

	/**
	 * the log-output-instance
	 */
	boost::shared_ptr<StatsOut> stats_out_;

	/*/**
	 * all the subsets under inspection - up to 9, but only those
	 * that were requested. They appear in order:
	 *
	 * [ALL] [ACTALL] [INACTALL]
	 * [MASTERS] [ACTMASTERS] [INACTMASTERS]
	 * [SLAVES] [ACTSLAVES] [INACTSLAVES]
	 *
	 * the inner vector holds shared pointers to the robots' data of that subset
	 */
	/*std::vector<std::vector<boost::shared_ptr<RobotData> > > cur_subsets_;*/

	/**
	 * whether or not the statistics-module is initialized.
	 * False before calling init() the first time
	 * and after calling quit().
	 */
	bool stats_initialized_;

	/*/**
	 * updates all the subsets in cur_subsets_
	 */
	/*void update_subsets();*/

	/**
	 * performs a calculation. That includes any update of the
	 */
	void calculate();

	/**
	 * performs the datadump - if requested.
	 */
	void do_datadump(const WorldInformation& world_information, boost::shared_ptr<Event> event);

	
	/*/**
	 * calculates the visibility graph for the given world information
	 * TODO(craupach) doing it here does not actually makes much sense with regard to the structure of
	 * the statistics module. Should be moved appropriately!!
	 *
	 *
	 * calculates the components of the visibility graph by placing the information from
	 * world_info and the according views into the boost graph data structure
	 * it then uses the boost component algorithm
	 *
	 * graph contains the visibility graph in the boost data structure
	 * vertices are integers which correspond to robot ids
	 * component contains a vector which maps robot ids to a component
	 *
	 * if graph is connected, returns 0, if it is not, it returns 1

	 */
	/*void calculate_visibility_graph(const WorldInformation& world_info,
			boost::shared_ptr<StatisticsDataObject> data);*/
};

#endif /* STATS_CONTROL_H_ */

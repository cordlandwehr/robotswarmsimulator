/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
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
 * \class	SimulationKernel
 * \author	Martina Hüllmann, modified by Alexander Setzer
 * \brief	The central module of the Swarm–Simulator. Manages the data
 * 			and the progress of the simulated world.
 *
 */

#ifndef SIMULATION_KERNEL_H_
#define SIMULATION_KERNEL_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <map>

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/VectorModifiers/vector_modifier.h"
#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/parser.h"

#include "../EventHandlers/request_handler.h"
#include "../EventHandlers/vector_request_handler.h"

#include "../Model/robot.h"
#include "../Model/world_modifier.h"

#include "../SimulationControl/history.h"
#include "../SimulationControl/time_point.h"

#include "../Statistics/stats_control.h"


class EventHandler;
class ActivationSequenceGenerator;


class SimulationKernel {
	friend class save_main_project_file_1;
	friend class save_robot_file_1;
	friend class write_robot_1;
	friend class simkernel_init;

public:
	SimulationKernel();
	~SimulationKernel();

	/**
	 * Returns a constant reference to the set of the robots.
	 * \return Constant reference to the set of the robots.
	 */
	const std::vector<boost::shared_ptr<Robot> >& robots() const;

	/**
	 * Returns a constant reference to History of WorldInformations.
	 * \return Constant reference to History of WorldInformations.
	 *
	 */
	const boost::shared_ptr<History>& history() const;

	/**
	 * This method initializes the simulation kernel
	 */
	void init(const std::string& project_filename, boost::shared_ptr<History> history, std::string output_dir, bool create_statistics);

	/**
	 * Method for performing one event-based step of the simulation
	 * by calling the current EventHandler
	 */
	void step();

	/**
	 * calls the step-method multiple times
	 * \param number of steps
	 */
	void multistep(int steps);

	/**
	 * Method called at simulation's termination for a clean
	 * closing of all resources (e.g. Statistics).
	 */
	void quit();

	/**
	 * Calls the Parser to save the current configuration
	 */
	void dump_simulation();

	std::string camera_position(){ return camera_position_; }
	std::string camera_direction(){ return camera_direction_;}
	std::string camera_type(){return  camera_type_;}

	/**
	 * This function checks a number of (well at the moment only one) terminate conditions.
	 * If one is fullfilled it returns true. The simulation can then be terminated from
	 * simulation control.
	 */
	bool terminate_condition(bool run_until_no_multiplicity) const;

private:

	/**
	 * Some enumerations which may be expanded on demand
	 */
	enum ASGType { SYNCHRONOUS,
	               SEMISYNCHRONOUS,
	               ASYNCHRONOUS };

	//TODO (dwonisch): I dont like this. When writing a new View classes you have to change
	//				   code in 3 places: Here in header file, in cc file at the view_map_,
	//				   and in cc file at the switch case.
	enum ViewType { FULLVIEW,
	                GLOBALVIEW };

	enum VectorModifierType { VECTOR_TRIMMER,
							  VECTOR_RANDOMIZER,
							  VECTOR_DIFFERENCE_TRIMMER	};

	enum RequestHandlerType { POSITION_REQUEST_HANDLER,
							  VELOCITY_REQUEST_HANDLER,
							  ACCELERATION_REQUEST_HANDLER,
							  TYPE_CHANGE_REQUEST_HANDLER,
							  MARKER_REQUEST_HANDLER,
							  COLOR_REQUEST_HANDLER};

	/**
	 * Map of robots in the world
	 */
	std::vector< boost::shared_ptr<Robot> > robots_;

	/**
	 * Set of world modifiers
	 */
	std::vector< boost::shared_ptr<WorldModifier> > world_modifiers_;

	/**
	 * Reference to a History of the WorldInformations.
	 */
	boost::shared_ptr<History> history_;

	/**
	 * Reference to the Parser
	 */
	boost::shared_ptr<Parser> parser_;

	/**
	 * Event Handler
	 */
	boost::shared_ptr<EventHandler> event_handler_;

	/**
	 * Activation sequence generator
	 */
	boost::shared_ptr<ActivationSequenceGenerator> asg_;

	/**
	 * Robot Control
	 */

	/**
	 * Statistics-Module
	 */
	boost::shared_ptr<StatsControl> stats_;

	/**
	 * Map for different types of ASG. This map is used to toggle between the
	 * String defined for the ASG in the projectfile and the right ASG class
	 */
	std::map<std::string, ASGType> ASG_map_;

	/**
	 * Map for different types of Views. This map is used to toggle between the
	 * String defined for the Views in the projectfile and the right View class
	 */
	std::map<std::string, ViewType> view_map_;

	/**
	 * Map for different stati of Robots. This map is used to toggle between the
	 * String defined for the Robotstatus in the robotfile and the enum for Robotstati
	 */
	std::map<std::string, RobotStatus> robot_status_map_;

	/**
	 * Map for different types of Robots. This map is used to toggle between the
	 * String defined for the Robotstatus in the robotfile and the enum for Robottypes
	 */
	std::map<std::string, RobotType> robot_type_map_;

	/**
	 * Map for different types of Vector Modifiers. This map is used to toggle between the
	 * String defined for the Vectormodifiers in the projectfile and the Modifiers.
	 */
	std::map<std::string, VectorModifierType> vector_modifier_map_;

	/**
	 * This method will do the dirty job in the init-method and construct the 1st world-information.
	 * \param 	Pointer to the Parser that stores all the information needed
	 * \return	the 1st world-info which Parser extracted from projectfile
	 */
	boost::shared_ptr<WorldInformation> setup_initial_world_information(boost::shared_ptr<Parser>);

	/**
	 * General method to setup any kind of request handler with all that stuff you need for it.
	 */
	boost::shared_ptr<RequestHandler> setup_request_handler(RequestHandlerType,
			unsigned int, double, boost::shared_ptr<History>, std::vector<std::string>);

	/**
	 * Genereal method to setup modifiers for vector-request-handlers.
	 */
	void setup_vectormodifier(boost::shared_ptr<VectorRequestHandler>, std::vector<std::string>);

	/**
	 * This method creates the robots using the information read from the robot input file.
	 */
	void create_robots(boost::shared_ptr<Parser> parser, boost::shared_ptr<WorldInformation> initial_world_information);
	
	/**
	 * This method creates the edges using the information read from the robot input file.
	 */
	void create_edges(boost::shared_ptr<Parser> parser, boost::shared_ptr<WorldInformation> initial_world_information);

	/**
	 * This method creates the world modifiers using the information read from the project file.
	 */
	void create_world_modifiers(boost::shared_ptr<Parser> parser);


	/**
	 * Used to do any last minute statistics before termination. We don't want to compute these
	 * Statistics at all steps for runtime reasons.
	 * TODO(craupach) this should integrated into statistics.
	 */
	void last_breath() const;

	std::string camera_position_;
	std::string camera_direction_;
	std::string camera_type_;

};

#endif /* SIMULATION_KERNEL_H_ */

/**
 * \class	SimulationKernel
 * \author	Martina Hüllmann
 * \brief	The central module of the Swarm–Simulator. Manages the data
 * 			and the progress of the simulated world.
 *
 */

#ifndef SIMULATION_KERNEL_H_
#define SIMULATION_KERNEL_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/tuple/tuple.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <map>

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/parser.h"

#include "../Model/robot.h"

#include "../SimulationControl/history.h"

#include "../Statistics/stats_control.h"


class EventHandler;
class ActivationSequenceGenerator;

using namespace std;

class SimulationKernel {
	friend class save_main_project_file_1;
	friend class save_robot_file_1;
	friend class write_obstacle_1;
	friend class write_robot_1;
public:
	SimulationKernel();
	~SimulationKernel();

	/**
	 * Returns a constant reference to the set of the robots.
	 * \return Constant reference to the set of the robots.
	 */
	const vector<boost::shared_ptr<Robot> >& robots() const;

	/**
	 * Returns a constant reference to History of WorldInformations.
	 * \return Constant reference to History of WorldInformations.
	 *
	 * TODO(martinah) return History& instead of boost::shared_ptr<History>&
	 * TODO(craupach) is this method needed?
	 * REMARK(mmarcus): I changed this the way martinah wanted?
	 */
	const boost::shared_ptr<History>& history() const;

	/**
	 * This method initializes the simulation kernel
	 */
	void init(const string& project_filename, boost::shared_ptr<History> history);

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

private:

	//TODO(mmarcus) enumerate more
	enum ASGType { SYNCHRONOUS,
	               SEMISYNCHRONOUS,
	               ASYNCHRONOUS };

	//TODO(mmarcus) enumerate more
	enum ViewType { FULLVIEW,
	                GLOBALVIEW };

	/**
	 * Set of robots in the world
	 */
	std::vector< boost::shared_ptr<Robot> > robots_;

	/**
	 * Reference to a History of the WorldInformations.
	 */
	boost::shared_ptr<History> history_;

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
	map<string, ASGType> ASG_map_;

	/**
	 * Map for different types of Views. This map is used to toggle between the
	 * String defined for the Views in the projectfile and the right View class
	 */
	map<string, ViewType> view_map_;

	/**
	 * Used to create an initial worldinformation from the information of the parser
	 * \return the worldinformation that the parser extracted from the projectfile
	 */
	boost::shared_ptr<WorldInformation> setup_initial_world_information(boost::shared_ptr<Parser>);

};

#endif /* SIMULATION_KERNEL_H_ */

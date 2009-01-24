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

#include "../Utilities/vector3d.h"


using namespace std;

class Identifier;
class WorldInformation;
class History;
class Obstacle;
class RobotData;
class Robot;
class WorldObject;

class SimulationKernel {
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

private:
	//TODO(martinah) comment variables
	//variables initialized with values in the main project file
	int asg_;
	string compass_model_;
	int event_handler_;
	string obstacle_filename_;
	string project_name_;
	string robot_filename_;
	int statistics_module_;

	/**
	 * Set of robots in the world
	 */
	std::vector< boost::shared_ptr<Robot> > robots_;

	/**
	 * Reference to a History of the WorldInformations.
	 */
	boost::shared_ptr<History> history_;

	/**
	 * This method saves the data to the predefined project files
	 * (main project file, robot file, obstacle file)
	 * \param Name of the main project file.
	 */
	void save_projectfiles(const string& project_filename);

	/**
	 * This method saves the following variables with their current
	 * values to the specified main project file:
	 * asg_, compass_model_, event_handler_, obstacle_filename_,
	 * project_name_, robot_filename_, statistics_module_
	 * \param Name of the main project file.
	 */
	void save_main_project_file(const string& project_filename);

	/**
	 * This method saves information about the robots.
	 */
	void save_robot_file();

	/**
	 * This method constructs a line for the robot_file which describes
	 * the current robot
	 * \param Pointer to the robot
	 */
	string write_robot(boost::shared_ptr<RobotData> robot_data);

	/**
	 * This method saves information about the obstacles
	 */
	void save_obstacle_file();

	/**
	 * This method constructs a line for the obstacle_file which describes
	 * the current obstacle
	 * \param Pointer to the obstacle
	 */
	string write_obstacle(boost::shared_ptr<Obstacle> current_obstacle);

	/**
	 * This method constructs a line for the obstacle_file which describes
	 * the current marker
	 * \param Pointer to the marker
	 */
	string write_marker(boost::shared_ptr<WorldObject> marker);

};

#endif /* SIMULATION_KERNEL_H_ */

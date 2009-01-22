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
#include <fstream>
#include <string>
#include <iostream>
#include <cstddef>
#include <map>


using namespace std;

class Robot;
class Identifier;
class WorldInformation;
class History;
class Obstacle;


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

	//TODO(martinah) add doxygen comments
	// TODO(craupach) are these methods needed / do they need to be public
	int asg() const { return asg_; }
	string compass_model() const { return compass_model_; }
	int event_handler() const { return event_handler_; }
	string obstacle_filename() const { return obstacle_filename_; }
	string project_name() const { return project_name_; }
	string robot_filename() const { return robot_filename_; }
	int statistics_module() const { return statistics_module_; }

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
	 * This method loads the data written in the given project files
	 * (main project file, robot file, obstacle file)
	 * and initializes the according variables.
	 * \param Name of the main project file.
	 */
	void load_projectfiles(const string& project_filename);

	/**
	 * This method loads the data written in the main project file
	 * and initializes the following variables:
	 * asg_, compass_model_, event_handler_, obstacle_filename_,
	 * project_name_, robot_filename_, statistics_module_
	 * \param Name of the main project file.
	 */
	void load_main_project_file(const string& project_filename);

	/**
	 * This method loads the data written in the robot file.
	 */
	void load_robot_file();

	/**
	 * This method loads the data written in the obstacle file.
	 */
	void load_obstacle_file();

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
	 * This method saves information about the obstacles
	 */
	void save_obstacle_file();

	/**
	 * This method checks whether the given line is a comment,
	 * i.e. begins with a '#'.
	 *
	 * \return True, if line is a comment.
	 */
	bool is_comment(const string& line);

	/**
	 * This methods returns the variable contained in the given line.
	 * Excepts, that the given line contains an assignment.
	 */
	string get_var_name(const string& line);

	/**
	 * This methods returns the value of the assignment in this line.
	 * Excepts, that the given line contains an assignment.
	 */
	string get_var_value(const string& line);

	/**
	 * This methods initializes all necessary variables with the values saved
	 * in the given map.
	 *
	 * \param Constant reference to the map containing variable names and its values.
	 *
	 * TODO(martinah) use a constant reference to map as input parameter
	 */
	void init_variables(map<string,string> variables_and_values);

	/**
	 * This methods returns the (string) value of the variable according to var_name saved in the given map
	 *
	 * \param variables_and_values Map that variable names and its values.
	 * \param var_name Name of the variable whose value shall be returned from the map.
	 * \return If var_name exists in the map, return (string) value of var_name saved in the map.
	 */
	string get_string_value_from_map(map<string,string> variables_and_values, const string& var_name);

	/**
	 * This methods returns the (int) value of the variable according to var_name saved in the given map
	 *
	 * \param variables_and_values Map that variable names and its values.
	 * \param var_name Name of the variable whose value shall be returned from the map.
	 * \return If var_name exists in the map, return (int) value of var_name saved in the map.
	 */
	int get_int_value_from_map(map<string,string> variables_and_values, const string& var_name);

	/**
	 * Checks whether the given string contains an assignment.
	 */
	bool contains_assignment(const string& line);

	/**
	 * Used to write the standard header for the obstacle file
	 * \param outputstream to obstacle file
	 */
	void write_obstacle_header(ofstream obstacle_file);

	/**
	 * This method writes the output to the obstaclefile
	 * \param obstacle_file	outputstream to obstacle file
	 * \param obs the obstacle to print
	 */
	void write_obstacle(ofstream obstacle_file, boost::shared_ptr<Obstacle>);
};

#endif /* SIMULATION_KERNEL_H_ */

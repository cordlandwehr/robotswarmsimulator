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
	 */
	const boost::shared_ptr<History>& history() const;

	/**
	 * This method initializes the simulation kernel
	 */
	void init(const string& project_filename);

	//TODO(martinah) add doxygen comments
	int asg() const { return asg_; }
	string algorithm() const { return algorithm_; }
	string compass_model() const { return compass_model_; }
	int event_handler() const { return event_handler_; }
	string obstacle_filename() const { return obstacle_filename_; }
	string project_name() const { return project_name_; }
	string robot_filename() const { return robot_filename_; }
	int statistics_module() const { return statistics_module_; }

private:
	//TODO(martinah) comment variables
	int asg_;
	string algorithm_;
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
	 * This methods loads the data written in the given project file
	 * and initializes the according variables.
	 */
	void load_projectfile(const string& project_filename);

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
};

#endif /* SIMULATION_KERNEL_H_ */

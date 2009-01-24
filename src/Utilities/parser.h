/**
 * \class	SimulationKernel
 * \author	Martina Hüllmann
 * \brief	Class for reading project files.
 *
 */

#ifndef PARSER_H_
#define PARSER_H_


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

class Parser {

	friend class loading_main_project_file_1;
	friend class loading_main_project_file_2;
	friend class loading_main_project_file_3;
public:
	Parser();
	virtual ~Parser();

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

#endif /* PARSER_H_ */

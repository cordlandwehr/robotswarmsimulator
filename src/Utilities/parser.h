/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
 * \brief	Class for reading and saving project files.
 *
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <map>

#include "../Model/sphere.h"
#include "../Model/box.h"
#include "../Model/robot_data.h"

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/identifier.h"

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/unsupported_operation_exception.h"


class Parser {

	//test cases for loading project files
	friend class LoadMainProjectFileTest;
	friend class load_robot_file_1;
	friend class load_edge_file_1;


public:
	Parser();
	virtual ~Parser();

	/**
	 * Returns a map which stores all parameters as boost::any.
	 * Parameter Names exactly as in Specification Document
	 * Exact Types of boost::any objects can be looked up in Specification Document
	 */
	std::map<std::string, std::string>& parameter_map();
	
	/**
	 * Returns a map which stores all parameters as boost::any.
	 * Parameter Names exactly as in Specification Document
	 * Exact Types of boost::any objects can be looked up in Specification Document
	 */
	boost::program_options::variables_map& parameter_map_boost() { return parameter_map_boost_;};	

	/**
	 * Set-method for project_name
	 * @param project_name string
	 */
	void set_project_name(const std::string& project_name);

	/**
	 * Set-method for robot filename
	 * @param robot_filename string
	 */
	void set_robot_filename(const std::string& robot_filename);

	/**
	 * Set-method for statistics_template
	 * @param statistics template string
	 */
	void set_statistics_template(const std::string& statistics_template);

	/**
	 * Set-method for project filename
	 * @param project_filename string
	 */
	void set_project_filename(const std::string& project_filename);

	/**
	 * This method loads the data written in the given project files
	 * (main project file, robot file)
	 * and initializes the according variables.
	 * \param Name of the main project file.
	 */
	void load_projectfiles(const std::string& project_filename);


	/**
	 * \return constant reference to the string describing the Robot Filename.
	 */
	const std::string& robot_filename() const;
	
	/**
	 * \return constant reference to the string describing the Edge Filename.
	 */
	const std::string& edge_filename() const;

	/**
	 * \return reference to vector of Robot IDs.
	 */
	std::vector<int>& robot_ids();	
	
	/**
	 * \return reference to vector of Robot Positions.
	 */
	std::vector<Vector3d>& robot_positions();

	/**
	 * \return reference to vector of Robot Marker Information.
	 */
	std::vector<std::string>& robot_marker_information();

	/**
	 * \return reference to vector of Robot algorithms.
	 */
	std::vector<std::string>& robot_algorithms();

	/**
	 * \return reference to vector of world modifier.
	 */
	std::vector<std::string>& world_modifiers();
	
	/**
	 * \return reference to vector of first edge ends
	 */
	std::vector<int>& edge_end1();	
	
	/**
	 * \return reference to vector of second edge ends
	 */
	std::vector<int>& edge_end2();		
	
	/**
	 * \return reference to vector of bidirectional booleans
	 */
	std::vector<int>& edge_bidirectional();	

	/**
	 * \returns the dumpnumber (for saving snapshots of configurations).
	 */
	int dumpnumber();

	/**
	 * \return the project path of the beforehand loaded project
	 */
	boost::filesystem::path get_project_path();

private:

	////////////////////////////////////////////////////////
	// SIMULATION CONFIGURATION
	////////////////////////////////////////////////////////

	//variables initialized with values in the main project file

	// parameter map
	std::map<std::string, std::string> parameter_map_;
	boost::program_options::variables_map parameter_map_boost_;

	std::string robot_filename_;
	std::string edge_filename_;
	std::vector<std::string> world_modifiers_;
	int dumpnumber_;

	//file name of project file
	std::string project_filename_;

	//position in line for getting values
	size_t position_in_line_;

	////////////////////////////////////////////////////////
	// ROBOT DATA
	////////////////////////////////////////////////////////
	std::vector<int> initiale_robot_ids_;
	std::vector<Vector3d> initiale_robot_positions_;
	std::vector<std::string> initiale_robot_marker_information_;
	std::vector<std::string> initiale_robot_algorithms_;
	std::vector<int> initiale_edge_end1_;
	std::vector<int> initiale_edge_end2_;
	std::vector<int> initiale_edge_bidirectional_;

	/**
	 * This method loads the data written in the main project file
	 * and initializes the following variables:
	 * asg_, compass_model_, event_handler_,
	 * project_name_, robot_filename_, statistics_module_
	 * \param Name of the main project file.
	 * \return a map which stores all parameters as boost::any. 
	 */
	void load_main_project_file(const std::string& project_filename);

	/**
	 * This method removes leading and trailing quotes if the given string is quoted.
	 * \param value String which may contain quotes to be removed.
	 * \return Given string without leading or trailing quotes.
	 */
	std::string remove_quotes_and_leading_and_trailing_spaces(const std::string& value);

	/**
	 * This method loads the data written in the robot file.
	 */
	void load_robot_file();
	
	/**
	 * This method loads the data written in the edge file.
	 */
	void load_edge_file();

	/**
	 * This method loads an obstacle or a robot file.
	 * \param load_robot_file true, if file to load is robot file
	 */
	void load_robot_or_edge_file(bool load_robot_file);

	/**
	 * This method checks whether the given line is a comment,
	 * i.e. begins with a '#'.
	 *
	 * \return True, if line is a comment.
	 */
	bool is_comment(const std::string& line);

	/**
	 * This methods initializes all necessary variables with the values saved
	 * in the given map.
	 *
	 * \param Constant reference to the map containing variable names and its values.
	 *
	 * TODO(martinah) use a constant reference to map as input parameter
	 */
	void init_variables();

	/**
	 * This methods returns the (string) value of the variable according to var_name saved in the given map.
	 * If var_name doesn't exist in the map, and var_name has a default value, default value will be returned.
	 *
	 * \param	 variables_and_values Map that contains variable names and its values.
	 * \param 	var_name Name of the variable whose value shall be returned from the map.
	 * \return 	If var_name exists in the map, return (string) value of var_name saved in the map.
	 * 			If var_name doesn't exist in the map, but var_name has a default value, return this default_value.
	 */
	std::string get_string_value_from_map(const boost::program_options::variables_map& variables_and_values, const std::string& var_name);

	/**
	 * Checks whether the given string contains an assignment.
	 */
	bool contains_assignment(const std::string& line);

	/**
	 * This methods reads the values in the given line and initializes the according variables.
	 * \param line Line containing information for one robot
	 * \param line_number number of given line in file
	 */
	void init_robot_values_for_line(const std::string& line, int line_number);
	
	/**
	 * This methods reads the values in the given line and initializes the according variables.
	 * \param line Line containing information for one edge
	 * \param line_number number of given line in file
	 */
	void init_edge_values_for_line(const std::string& line, int line_number);

	/**
	 * This methods returns the next value in the given line (beginning at given postion).
	 * \param line 	Line with values
	 * \param pos	Position from where to get next value
	 * \param line_number Number of current line (needed for error message)
	 * \param last_value Denotes whether the value to read is the last value of this line (i.e. this value
	 * 				is not limited by the seperator specified, but by the end of line)
	 * \return Next value in given line.
	 */
	std::string get_next_value_in_line(const std::string& line, int line_number, bool last_value);

	/**
	 * This method returns the next double value in given line.
	 * \param line 	Line with values
	 * \param line_number Number of current line (needed for error message)
	 * \param last_value Denotes whether the value to read is the last value of this line (i.e. this value
	 * 				is not limited by the seperator specified, but by the end of line)
	 * \return Next double in given line.
	 */
	double get_next_double_value_in_line(const std::string& line, int line_number, bool last_value);

	/**
	 * This methods reads the next 3 values from line and returns a vector3d containing these values.
	 * \param line 	Line with values
	 * \param pos	Position from where to get next values
	 * \param line_number Number of current line (needed for error message)
	 * \param last_value Denotes whether the values to read are the last values of this line (i.e. the third value
	 * 				is not limited by the seperator specified, but by the end of line)
	 * \return Vector3d containing the next 3 values in line.
	 */
	Vector3d get_next_vector3d_in_line(const std::string& line, int line_number, bool last_value);

	/**
	 * This method converts the given string to double (if given string represents a double value)
	 * \return double value of given string
	 */
	double string_to_double(const std::string& my_string);

// 
};

#endif /* PARSER_H_ */

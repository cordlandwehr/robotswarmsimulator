/**
 * \class	SimulationKernel
 * \author	Martina Hüllmann
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
	friend class load_obstacle_file_1;

	//test cases for saving project files
	friend class save_main_project_file_1;
	friend class write_robot_1;

public:
	Parser();
	virtual ~Parser();

	/**
	 * Returns a map which stores all parameters as boost::any.
	 * Parameter Names exactly as in Specification Document
	 * Exact Types of boost::any objects can be looked up in Specification Document
	 */
	std::map<std::string, std::string>& parameter_map() { return parameter_map_;};

	/**
	 * Set-method for compass model
	 * @param compass_model string
	 */
	void set_compass_model(const std::string& compass_model);

	/**
	 * Set-method for obstacle filename
	 * @param obstacle_filename string
	 */
	void set_obstacle_filename(const std::string& obstacle_filename);

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
	 * Set-method for statistics_subsets
	 * @param statistics subsets string
	 */
	void set_statistics_subsets(const std::string& statistics_subsets);

	/**
	 * Set-method for project filename
	 * @param project_filename string
	 */
	void set_project_filename(const std::string& project_filename);

	/**
	 * This method loads the data written in the given project files
	 * (main project file, robot file, obstacle file)
	 * and initializes the according variables.
	 * \param Name of the main project file.
	 */
	void load_projectfiles(const std::string& project_filename);

	/**
	 * This method saves the data to the predefined project files
	 * (main project file, robot file, obstacle file)
	 * \param Name of the main project file.
	 */
	void save_projectfiles(const std::string& project_filename, const WorldInformation& world_info);

	/**
	 * This method sets default variables and values.
	 */
	void init();

	/**
	 * \return constant reference to the string describing the type of the Compass Model in the parser
	 */
	const std::string& compass_model() const;

	/**
	 * \return constant reference to the string describing the Robot Filename.
	 */
	const std::string& robot_filename() const;

	/**
	 * \return reference to vector of Robot Positions.
	 */
	std::vector<Vector3d>& robot_positions();

	/**
	 * \return reference to vector of Robot Velocites.
	 */
	std::vector<Vector3d>& robot_velocities();

	/**
	 * \return reference to vector of Robot Accelerations.
	 */
	std::vector<Vector3d>& robot_accelerations();

	/**
	 * \return reference to vector of Robot Types.
	 */
	std::vector<std::string>& robot_types();

	/**
	 * \return reference to vector of Robot Stati.
	 */
	std::vector<std::string>& robot_stati();

	/**
	 * \return reference to vector of Robot Marker Information.
	 */
	std::vector<std::string>& robot_marker_information();

	/**
	 * \return reference to vector of Robot algorithms.
	 */
	std::vector<std::string>& robot_algorithms();

	/**
	 * \return reference to vector of Robot color.
	 */
	std::vector<std::string>& robot_colors();

	/**
	 * \return reference to 3-tuple of Robot coordinate axis.
	 */
	std::vector<boost::tuple<Vector3d, Vector3d, Vector3d > >& robot_coordinate_systems();

	/**
	 * \return reference to vector of Obstacle types.
	 */
	std::vector<std::string>& obstacle_types();

	/**
	 * \return reference to vector of Obstacle positions.
	 */
	std::vector<Vector3d>& obstacle_positions();

	/**
	 * \return reference to vector of Obstacle Marker information
	 */
	std::vector<std::string>& obstacle_marker_information();

	/**
	 * \return reference to vector of Obstacle radius.
	 */
	std::vector<double>& obstacle_radius();

	/**
	 * \return reference to vector of Obstacle size.
	 */
	std::vector<Vector3d>& obstacle_size();

	/**
	 * \returns the dumpnumber (for saving snapshots of configurations).
	 */
	int dumpnumber();

private:

	////////////////////////////////////////////////////////
	// SIMULATION CONFIGURATION
	////////////////////////////////////////////////////////

	//variables initialized with values in the main project file

	// parameter map
	std::map<std::string, std::string> parameter_map_;

	std::string robot_filename_;
	std::string obstacle_filename_;
	int dumpnumber_;

	//file name of project file
	std::string project_filename_;

	//position in line for getting values
	size_t position_in_line_;

	////////////////////////////////////////////////////////
	// ROBOT DATA
	////////////////////////////////////////////////////////
	std::vector<Vector3d> initiale_robot_positions_;
	std::vector<Vector3d> initiale_robot_velocities_;
	std::vector<Vector3d> initiale_robot_accelerations_;
	std::vector<std::string> initiale_robot_types_;
	std::vector<std::string> initiale_robot_stati_;
	std::vector<std::string> initiale_robot_marker_information_;
	std::vector<std::string> initiale_robot_algorithms_;
	std::vector<std::string> initiale_robot_colors_;
	std::vector<boost::tuple<Vector3d, Vector3d, Vector3d> > initiale_robot_coordinate_systems_;

	////////////////////////////////////////////////////////
	// OBSTACLE DATA
	////////////////////////////////////////////////////////
	std::vector<std::string> initiale_obstacle_types_;
	std::vector<Vector3d> initiale_obstacle_positions_;
	std::vector<std::string> initiale_obstacle_marker_information_;
	std::vector<double> initiale_obstacle_radius_;
	std::vector<Vector3d> initiale_obstacle_size_;

	/**
	 * This method returns the default value of the given variable, if the given variable has a default value,
	 * otherwise "" will be returned.
	 * \param var Variable which default value will be returned.
	 * \return Default value of given variable, if given variable has a default value, otherwise "".
	 */
	std::string get_default_value(const std::string& var);

	/**
	 * This method loads the data written in the main project file
	 * and initializes the following variables:
	 * asg_, compass_model_, event_handler_, obstacle_filename_,
	 * project_name_, robot_filename_, statistics_module_
	 * \param Name of the main project file.
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
	 * This method loads the data written in the obstacle file.
	 */
	void load_obstacle_file();

	/**
	 * This method loads an obstacle or a robot file.
	 * \param load_robot_file true, if file to load is robot file
	 */
	void load_robot_or_obstacle_file(bool load_robot_file);

	/**
	 * This method checks whether the given line is a comment,
	 * i.e. begins with a '#'.
	 *
	 * \return True, if line is a comment.
	 */
	bool is_comment(const std::string& line);

	/**
	 * This methods returns the variable contained in the given line.
	 * Excepts, that the given line contains an assignment.
	 */
	std::string get_var_name(const std::string& line);

	/**
	 * This methods returns the value of the assignment in this line.
	 * Returns default value of variable, if variable has a default value and
	 * variable name isn't set in input file.
	 * \param line Line from file containing an assignment.
	 * \param name Variable name in line.
	 */
	std::string get_var_value(const std::string& line, const std::string& name);


	/**
	 * This methods initializes all necessary variables with the values saved
	 * in the given map.
	 *
	 * \param Constant reference to the map containing variable names and its values.
	 *
	 * TODO(martinah) use a constant reference to map as input parameter
	 */
	void init_variables(std::map<std::string,std::string> variables_and_values);

	/**
	 * This methods returns the (string) value of the variable according to var_name saved in the given map.
	 * If var_name doesn't exist in the map, and var_name has a default value, default value will be returned.
	 *
	 * \param	 variables_and_values Map that contains variable names and its values.
	 * \param 	var_name Name of the variable whose value shall be returned from the map.
	 * \return 	If var_name exists in the map, return (string) value of var_name saved in the map.
	 * 			If var_name doesn't exist in the map, but var_name has a default value, return this default_value.
	 */
	std::string get_string_value_from_map(std::map<std::string,std::string> variables_and_values, const std::string& var_name);

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
	 * This methods reads the in the given line and initializes the according variables.
	 * \param line Line containing information for one obstacle
	 * \param line_number number of given line in file
	 */
	void init_obstacle_values_for_line(const std::string& line, int line_number);

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

	/**
	 * This method saves the following variables with their current
	 * values to the specified main project file:
	 * asg_, compass_model_, event_handler_, obstacle_filename_,
	 * project_name_, robot_filename_, statistics_module_
	 * \param Name of the main project file.
	 */
	void save_main_project_file(const std::string& project_filename);

	/**
	 * This method saves information about the robots.
	 * \Param world_info needed to get the current sets of robots
	 */
	void save_robot_file(const WorldInformation& world_info);

	/**
	 * This method constructs a line for the robot_file which describes
	 * the current robot
	 * \param Pointer to the robot
	 */
	std::string write_robot(boost::shared_ptr<RobotData> robot_data);

	/**
	 * This method saves information about the obstacles
	 * Param world_info needed to get the current sets of obstacles
	 */
	void save_obstacle_file(const WorldInformation& world_info);

	/**
	 * This method constructs a line for the obstacle_file which describes
	 * the current obstacle
	 * \param Pointer to the obstacle
	 */
	std::string write_obstacle(boost::shared_ptr<Obstacle> current_obstacle);

	/**
	 * This method constructs a line for the obstacle_file which describes
	 * the current marker
	 * \param Pointer to the marker
	 */
	std::string write_marker(boost::shared_ptr<WorldObject> marker);
};

#endif /* PARSER_H_ */

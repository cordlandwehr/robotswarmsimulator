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

#include <fstream>
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

using namespace std;

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
	 * TODO(craupach) should be implemented by the Kernel Team as principal interface to the parser
	 */
	std::map<std::string, std::string>& parameter_map() { return parameter_map_;};


	/**
	 * Set-method for ASG
	 * TODO is there an enum for ASG?
	 * REMARK(mmarcus) I made some dummy-enum in the simulationkernel...
	 * @param asg int
	 */
	void set_asg(const string& asg);

	/**
	 * Set-method for compass model
	 * @param compass_model string
	 */
	void set_compass_model(const string& compass_model);

	/**
	 * Set-method for obstacle filename
	 * @param obstacle_filename string
	 */
	void set_obstacle_filename(const string& obstacle_filename);

	/**
	 * Set-method for project_name
	 * @param project_name string
	 */
	void set_project_name(const string& project_name);

	/**
	 * Set-method for robot filename
	 * @param robot_filename string
	 */
	void set_robot_filename(const string& robot_filename);

	/**
	 * Set-method for statistics_template
	 * @param statistics template string
	 */
	void set_statistics_template(const string& statistics_template);

	/**
	 * Set-method for statistics_subsets
	 * @param statistics subsets string
	 */
	void set_statistics_subsets(const string& statistics_subsets);

	/**
	 * Set-method for project filename
	 * @param project_filename string
	 */
	void set_project_filename(const string& project_filename);

	/**
	 * This method loads the data written in the given project files
	 * (main project file, robot file, obstacle file)
	 * and initializes the according variables.
	 * \param Name of the main project file.
	 */
	void load_projectfiles(const string& project_filename);

	/**
	 * This method saves the data to the predefined project files
	 * (main project file, robot file, obstacle file)
	 * \param Name of the main project file.
	 */
	void save_projectfiles(const string& project_filename, const WorldInformation& world_info);

	/**
	 * This method sets default variables and values.
	 */
	void init();

	/**
	 * \return constant reference to the string describing the type of the ASG in the parser
	 */
	const string& asg() const;

	/**
	 * \return constant reference to the string describing the type of the Compass Model in the parser
	 */
	const string& compass_model() const;

	/**
	 * \return constant reference to the string describing the Projectname.
	 */
	const string& project_name() const;

	/**
	 * \return constant reference to the string describing the Robot Filename.
	 */
	const string& robot_filename() const;

	/**
	 * \return constant reference to the string describing the statistics' configuration-template
	 */
	const string& statistics_template() const;

	/**
	 * \return constant reference to the string describing the statistics' subsets-configuration
	 */
	const string& statistics_subsets() const;

	/**
	 * \return constant reference to the string describing the type of the used View in the parser
	 */
	const string& view() const;

	/**
	 * \return Seed of the Marker Request Handler.
	 */
	const unsigned int marker_request_handler_seed() const;

	/**
	 * \return Seed of the Type Change Request Handler.
	 */
	const unsigned int type_change_request_handler_seed() const;

	/**
	 * \return Seed of the Velocity Change Request Handler.
	 */
	const unsigned int velocity_request_handler_seed() const;

	/**
	 * \return Seed of the Position Change Request Handler.
	 */
	const unsigned int position_request_handler_seed() const;

	/**
	 * \return Seed of the Acceleration Change Request Handler.
	 */
	const unsigned int acceleration_request_handler_seed() const;

	/**
	 * \return discard probability of the Marker Request Handler.
	 */
	const double marker_request_handler_discard_prob() const;

	/**
	 * \return discard probability of the Type Change Request Handler.
	 */
	const double type_change_request_handler_discard_prob() const;

	/**
	 * \return discard probability of the Velocity Request Handler.
	 */
	const double velocity_request_handler_discard_prob() const;

	/**
	 * \return discard probability of the Position Request Handler.
	 */
	const double position_request_handler_discard_prob() const;

	/**
	 * \return discard probability of the Acceleration Request Handler.
	 */
	const double acceleration_request_handler_discard_prob() const;

	/**
	 * \return constant reference to vector of modifiers for the Velocity Request Handler.
	 */
	const vector<string>& velocity_request_handler_vector_modifier() const;

	/**
	 * \return constant reference to vector of modifiers for the Position Request Handler.
	 */
	const vector<string>& position_request_handler_vector_modifier() const;

	/**
	 * \return constant reference to vector of modifiers for the Acceleration Request Handler.
	 */
	const vector<string>& acceleration_request_handler_vector_modifier() const;

	/**
	 * \return reference to vector of Robot Positions.
	 */
	vector<Vector3d>& robot_positions();

	/**
	 * \return reference to vector of Robot Velocites.
	 */
	vector<Vector3d>& robot_velocities();

	/**
	 * \return reference to vector of Robot Accelerations.
	 */
	vector<Vector3d>& robot_accelerations();

	/**
	 * \return reference to vector of Robot Types.
	 */
	vector<string>& robot_types();

	/**
	 * \return reference to vector of Robot Stati.
	 */
	vector<string>& robot_stati();

	/**
	 * \return reference to vector of Robot Marker Information.
	 */
	vector<string>& robot_marker_information();

	/**
	 * \return reference to vector of Robot algorithms.
	 */
	vector<string>& robot_algorithms();

	/**
	 * \return reference to vector of Robot color.
	 */
	vector<string>& robot_colors();

	/**
	 * \return reference to 3-tuple of Robot coordinate axis.
	 */
	vector<boost::tuple<Vector3d, Vector3d, Vector3d > >& robot_coordinate_systems();

	/**
	 * \return reference to vector of Obstacle types.
	 */
	vector<string>& obstacle_types();

	/**
	 * \return reference to vector of Obstacle positions.
	 */
	vector<Vector3d>& obstacle_positions();

	/**
	 * \return reference to vector of Obstacle Marker information
	 */
	vector<string>& obstacle_marker_information();

	/**
	 * \return reference to vector of Obstacle radius.
	 */
	vector<double>& obstacle_radius();

	/**
	 * \return reference to vector of Obstacle size.
	 */
	vector<Vector3d>& obstacle_size();

private:

	////////////////////////////////////////////////////////
	// SIMULATION CONFIGURATION
	////////////////////////////////////////////////////////

	//variables initialized with values in the main project file

	// temporary parameter map
	std::map<std::string, std::string> parameter_map_;

	string asg_;
	string compass_model_;
	string obstacle_filename_;
	string project_name_;
	string robot_filename_;
	string statistics_template_;
	string statistics_subsets_;
	string view_;

	//Request Handler without Vector Modifiers
	//TypeChangeRequestHandler
	//MarkerRequestHandler

	//Request Handler with Vector Modifiers
	//PosistionRequestHandler
	//AccelerationRequestHandler
	//VelocityRequestHandler

	//A request handler is a tuple of the form
	//(TYPE,DISCARD_PROB,SEED,[VECTOR_MODIFIERS])
	//VECTOR_MODIFIERS is a list (resp. vector) with elements that are tuples of the form
	//(VECTOR_MODIFIER_TYPE,VECTOR_MODIFIER_PARAM_1,VECTOR_MODIFIER_PARAM_2,..).
	//For further information see users guide.

	boost::tuple<
		string,						//type of request handler
		double,						//discard probability
		unsigned int				//seed
		>
	type_change_request_handler_, marker_request_handler_;

	boost::tuple<
		string,						//type of request handler
		double,						//discard probability
		unsigned int,				//seed
		std::vector<boost::tuple<	//list of vector modifiers
			string,						//type of vector modifier
			std::vector<boost::any>		//parameters of vector modifiers
		> > >
	position_request_handler_, acceleration_request_handler_, velocity_request_handler_;

	//TODO(martinah) remove (but first modify methods for saving project files)
	//seeds
	unsigned int marker_request_handler_seed_;
	unsigned int type_change_request_handler_seed_;
	unsigned int velocity_request_handler_seed_;
	unsigned int position_request_handler_seed_;
	unsigned int acceleration_request_handler_seed_;

	//discard probabilities
	double marker_request_handler_discard_prob_;
	double type_change_request_handler_discard_prob_;
	double velocity_request_handler_discard_prob_;
	double position_request_handler_discard_prob_;
	double acceleration_request_handler_discard_prob_;

	//vector modifiers
	vector<string> velocity_request_handler_vector_modifier_;
	vector<string> position_request_handler_vector_modifier_;
	vector<string> acceleration_request_handler_vector_modifier_;

	//file name of project file
	string project_filename_;

	//default values (if variable not defined or set in input file)
	vector<string> variables_with_default_values;
	vector<string> default_values_of_varialbes;

	//position in line for getting values
	size_t position_in_line_;

	////////////////////////////////////////////////////////
	// ROBOT DATA
	////////////////////////////////////////////////////////
	//TODO(martinah) maybe adapt types of some variables
	std::vector<Vector3d> initiale_robot_positions_;
	std::vector<Vector3d> initiale_robot_velocities_;
	std::vector<Vector3d> initiale_robot_accelerations_;
	std::vector<string> initiale_robot_types_;
	std::vector<string> initiale_robot_stati_;
	std::vector<string> initiale_robot_marker_information_;
	std::vector<string> initiale_robot_algorithms_;
	std::vector<string> initiale_robot_colors_;
	std::vector<boost::tuple<Vector3d, Vector3d, Vector3d> > initiale_robot_coordinate_systems_;

	////////////////////////////////////////////////////////
	// OBSTACLE DATA
	////////////////////////////////////////////////////////
	//TODO(martinah) maybe adapt types of some variables
	std::vector<string> initiale_obstacle_types_;
	std::vector<Vector3d> initiale_obstacle_positions_;
	std::vector<string> initiale_obstacle_marker_information_;
	std::vector<double> initiale_obstacle_radius_;
	std::vector<Vector3d> initiale_obstacle_size_;

	/**
	 * This methods splits the string by the given seperator.
	 *
	 * \param my_string String to split
	 * \param seperator Seperator where to split given string
	 * \param max_num_pieces Number of pieces maximum allowed
	 * \return vector containing splitted string.
	 */
	std::vector<string> split_string_by_string(const string& my_string, const string& sep, int max_num_pieces);

	/**
	 * This method returns the default value of the given variable, if the given variable has a default value,
	 * otherwise "" will be returned.
	 * \param var Variable which default value will be returned.
	 * \return Default value of given variable, if given variable has a default value, otherwise "".
	 */
	string get_default_value(const string& var);

	/**
	 * This method loads the data written in the main project file
	 * and initializes the following variables:
	 * asg_, compass_model_, event_handler_, obstacle_filename_,
	 * project_name_, robot_filename_, statistics_module_
	 * \param Name of the main project file.
	 */
	void load_main_project_file(const string& project_filename);

	/**
	 * This method removes leading and trailing quotes if the given string is quoted.
	 * \param value String which may contain quotes to be removed.
	 * \return Given string without leading or trailing quotes.
	 */
	string remove_quotes_and_leading_and_trailing_spaces(const string& value);

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
	bool is_comment(const string& line);

	/**
	 * This methods returns the variable contained in the given line.
	 * Excepts, that the given line contains an assignment.
	 */
	string get_var_name(const string& line);

	/**
	 * This methods returns the value of the assignment in this line.
	 * Returns default value of variable, if variable has a default value and
	 * variable name isn't set in input file.
	 * \param line Line from file containing an assignment.
	 * \param name Variable name in line.
	 */
	string get_var_value(const string& line, const string& name);


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
	 * This methods returns the (string) value of the variable according to var_name saved in the given map.
	 * If var_name doesn't exist in the map, and var_name has a default value, default value will be returned.
	 *
	 * \param	 variables_and_values Map that contains variable names and its values.
	 * \param 	var_name Name of the variable whose value shall be returned from the map.
	 * \return 	If var_name exists in the map, return (string) value of var_name saved in the map.
	 * 			If var_name doesn't exist in the map, but var_name has a default value, return this default_value.
	 */
	string get_string_value_from_map(map<string,string> variables_and_values, const string& var_name);

	/**
	 * This methods returns the (int) value of the variable according to var_name saved in the given map
	 *
	 * \param variables_and_values Map that contains variable names and its values.
	 * \param var_name Name of the variable whose value shall be returned from the map.
	 * \return If var_name exists in the map, return (int) value of var_name saved in the map.
	 */
	unsigned int get_uint_value_from_map(map<string,string> variables_and_values, const string& var_name);

	/**
	 * This methods returns the (double) value of the variable according to var_name saved in the given map
	 *
	 * \param variables_and_values Map that contains variable names and its values.
	 * \param var_name Name of the variable whose value shall be returned from the map.
	 * \return If var_name exists in the map, return (double) value of var_name saved in the map.
	 */
	double get_double_value_from_map(map<string,string> variables_and_values, const string& var_name);

	/**
	 * This methods returns the vector of the variable according to var_name saved in the given map
	 *
	 * \param variables_and_values Map that contains variable names and its values.
	 * \param var_name Name of the variable whose vector shall be returned from the map.
	 * \return If var_name exists in the map, return vector of var_name saved in the map.
	 */
	std::vector<string> get_vector_from_map(map<string,string> variables_and_values, const string& var_name);

	/**
	 * This method returns a tuple that defines a request handler.
	 *
	 * \param variables_and_values Map that contains variable names and its values.
	 * \param request_handler Name of request handler to return.
	 * \param vector_modifier_exists Denotes wheter the request handler to return has a vector modifier list.
	 * \return Tuple that defines request handler with vector modifier
	 */
	boost::tuple<string,double,unsigned int,std::vector<boost::tuple<string,std::vector<boost::any> > > >
	get_request_handler_from_map(map<string,string> variables_and_values, const string& request_handler, bool vector_modifier_exists);

	/**
	 * This method returns a tuple that defines a request handler with vector modifiers.
	 *
	 * \param variables_and_values Map that contains variable names and its values.
	 * \param request_handler Name of request handler to return.
	 * \return Tuple that defines request handler without vector modifier
	 */
	boost::tuple<string,double,unsigned int,std::vector<boost::tuple<string,std::vector<boost::any> > > >
	get_request_handler_with_vector_modifiers_from_map(map<string,string> variables_and_values, const string& request_handler);


	/**
	 * This method returns a tuple that defines a request handler without vector modifiers.
	 *
	 * \param variables_and_values Map that contains variable names and its values.
	 * \param request_handler Name of request handler to return.
	 * \return Tuple that defines request handler without vector modifier
	 */
	boost::tuple<string,double,unsigned int>
	get_request_handler_without_vector_modifiers_from_map(map<string,string> variables_and_values, const string& request_handler);

	/**
	 * Checks whether the given string contains an assignment.
	 */
	bool contains_assignment(const string& line);

	/**
	 * This methods reads the values in the given line and initializes the according variables.
	 * \param line Line containing information for one robot
	 * \param line_number number of given line in file
	 */
	void init_robot_values_for_line(const string& line, int line_number);

	/**
	 * This methods reads the in the given line and initializes the according variables.
	 * \param line Line containing information for one obstacle
	 * \param line_number number of given line in file
	 */
	void init_obstacle_values_for_line(const string& line, int line_number);

	/**
	 * This methods returns the next value in the given line (beginning at given postion).
	 * \param line 	Line with values
	 * \param pos	Position from where to get next value
	 * \param line_number Number of current line (needed for error message)
	 * \param last_value Denotes whether the value to read is the last value of this line (i.e. this value
	 * 				is not limited by the seperator specified, but by the end of line)
	 * \return Next value in given line.
	 */
	string get_next_value_in_line(const string& line, int line_number, bool last_value);

	/**
	 * This method returns the next double value in given line.
	 * \param line 	Line with values
	 * \param line_number Number of current line (needed for error message)
	 * \param last_value Denotes whether the value to read is the last value of this line (i.e. this value
	 * 				is not limited by the seperator specified, but by the end of line)
	 * \return Next double in given line.
	 */
	double get_next_double_value_in_line(const string& line, int line_number, bool last_value);

	/**
	 * This methods reads the next 3 values from line and returns a vector3d containing these values.
	 * \param line 	Line with values
	 * \param pos	Position from where to get next values
	 * \param line_number Number of current line (needed for error message)
	 * \param last_value Denotes whether the values to read are the last values of this line (i.e. the third value
	 * 				is not limited by the seperator specified, but by the end of line)
	 * \return Vector3d containing the next 3 values in line.
	 */
	Vector3d get_next_vector3d_in_line(const string& line, int line_number, bool last_value);

	/**
	 * This method converts the given string to double (if given string represents a double value)
	 * \return double value of given string
	 */
	double string_to_double(const string& my_string);

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
	 * \Param world_info needed to get the current sets of robots
	 */
	void save_robot_file(const WorldInformation& world_info);

	/**
	 * This method constructs a line for the robot_file which describes
	 * the current robot
	 * \param Pointer to the robot
	 */
	string write_robot(boost::shared_ptr<RobotData> robot_data);

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
	string write_obstacle(boost::shared_ptr<Obstacle> current_obstacle);

	/**
	 * This method constructs a line for the obstacle_file which describes
	 * the current marker
	 * \param Pointer to the marker
	 */
	string write_marker(boost::shared_ptr<WorldObject> marker);
};

#endif /* PARSER_H_ */

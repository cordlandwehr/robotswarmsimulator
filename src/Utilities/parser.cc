#include "parser.h"

Parser::Parser() {
	// TODO Auto-generated constructor stub

}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

void Parser::init() {
	//define default values
	variables_with_default_values.push_back("COMPASS_MODEL");
	default_values_of_varialbes.push_back("FULL_COMPASS");

	variables_with_default_values.push_back("ASG");
	default_values_of_varialbes.push_back("ASG_SYNCHRON");

	variables_with_default_values.push_back("STATISTICS_MODULE");
	default_values_of_varialbes.push_back("NONE");

	variables_with_default_values.push_back("EVENT_HANDLER");
	default_values_of_varialbes.push_back("EH_DO_ALL");
}

bool Parser::is_comment(const string& line) {
	return (line.at(0)=='#');
}

bool Parser::contains_assignment(const string& line) {
	unsigned int pos_of_equal_sign = line.find_first_of("=");
	return pos_of_equal_sign != string::npos;
}

string Parser::get_var_name(const string& line) {
	unsigned int pos_of_equal_sign = line.find_first_of("=");

	//check if equal sign exists in line
	if(pos_of_equal_sign == string::npos) {
		throw UnsupportedOperationException("Syntax error in main project file. Line \""+line+"\" doesn't contain a "
				"valid assignment.");
	}

	//extract variable name from line
	string var_name = line.substr(0,pos_of_equal_sign);

	//get rid of unwanted leading and trailing spaces
	boost::trim(var_name);
	return var_name;
}

string Parser::get_default_value(const string& var) {
	for(int i=0; i<variables_with_default_values.size(); i++) {
		if(!var.compare(variables_with_default_values[i]) ||
		   !var.compare("ROBOT_FILENAME") ||
		   !var.compare("OBSTACLE_FILENAME") ) {

			//variable has a default value
			if(!var.compare("ROBOT_FILENAME")) {
				return project_filename_;
			}
			else if (!var.compare("OBSTACLE_FILENAME")) {
				return project_filename_;
			}
			else {
				return default_values_of_varialbes[i];
			}
		}
	}
	return "";
}

string Parser::get_var_value(const string& line, const string& name) {
	size_t pos_of_first_quote_sign = line.find_first_of("\"");
	size_t pos_of_last_quote_sign = line.find_last_of("\"");

	//check if value is correctly quoted and value exists
	if(pos_of_first_quote_sign == string::npos || pos_of_last_quote_sign == string::npos) {
		//get default value, if one exists
		string default_value = get_default_value(name);
		if(!default_value.empty()) {
			return default_value;
		}
		else {
			throw UnsupportedOperationException("Syntax error in main project file. Either variable value in line "
				"\""+line+"\" is not quote correctly or value is not defined.");
		}
	}

	return line.substr(pos_of_first_quote_sign+1,
			           pos_of_last_quote_sign-pos_of_first_quote_sign-1);

}

string Parser::get_string_value_from_map(map<string,string> variables_and_values,
													const string& var_name) {
	//get iterator to value to be searched for
	map<string,string>::const_iterator map_iterator = variables_and_values.find(var_name);

	//check if var_name exists in given map
	if(map_iterator == variables_and_values.end()) {
		//check if var_name has a default value
		string default_value = get_default_value(var_name);
		if(!default_value.empty()) {
			//variable has a default value
			return default_value;
		}
		else {
			//variable hasn't a default value and hasn't been initialized
			throw UnsupportedOperationException("Variable according to "+var_name+" has not been initialized and"
					"no default value for this variable exists.");
		}
	} else {
		//return value of var_name
		return variables_and_values.find(var_name)->second;
	}
}

int Parser::get_int_value_from_map(map<string,string> variables_and_values,
													const string& var_name) {
	//get string value from map
	string string_value = get_string_value_from_map(variables_and_values, var_name);

	//convert string value to int
	try {
		return boost::lexical_cast<int>(string_value);

	} catch(const boost::bad_lexical_cast& ) {
		throw UnsupportedOperationException("Failed casting string to integer.");
	}
}

void Parser::init_variables(map<string,string> variables_and_values) {

	//Variable names saved in the map are specified in the "Projectfiles Specification"-document
	asg_= get_int_value_from_map(variables_and_values, "ASG");
	compass_model_ = get_string_value_from_map(variables_and_values, "COMPASS_MODEL");
	event_handler_ = get_int_value_from_map(variables_and_values, "EVENT_HANDLER");
	obstacle_filename_ = get_string_value_from_map(variables_and_values, "OBSTACLE_FILENAME");
	project_name_ = get_string_value_from_map(variables_and_values, "PROJECT_NAME");
	robot_filename_ = get_string_value_from_map(variables_and_values, "ROBOT_FILENAME");
	statistics_module_ = get_string_value_from_map(variables_and_values, "STATISTICS_MODULE");
}

void Parser::load_main_project_file(const string& project_filename) {
	string line;
	ifstream project_file;
	project_filename_ = project_filename;
	string main_project_filename = project_filename_ + ".swarm";
	map<string, string> variables_and_values;

	project_file.open(main_project_filename.c_str());
	if(project_file.is_open()) {
		string var_name;
		string var_value;
		while(!project_file.eof()) {
			//read file line by line
			getline(project_file, line);

			//check whether current line isn't a comment and contains an assignment
			if(!is_comment(line) && contains_assignment(line)) {
				//get variable name
				var_name = get_var_name(line);

				//get variable value
				var_value = get_var_value(line, var_name);

				//insert variable and value into map
				variables_and_values.insert(pair<string,string>(var_name,var_value));
			}
		}
		project_file.close();

		//up to now: read all variables and its values from project file and
		//inserted them into a map

		//initialize variables
		init_variables(variables_and_values);

	} else {
		throw UnsupportedOperationException("Unable to open given project file.");
	}
}

string Parser::get_next_value_in_line(const string& line, int line_number, bool last_value) {
	//character that seperates values in line
	char seperator = ',';
	string value;

	size_t pos_of_next_seperator = line.find_first_of(seperator, position_in_line_);

	//check if there exists a next seperator
	if(pos_of_next_seperator == string::npos) {

		if(last_value) {
			//value to read is last value => return end of line beginning at given position
			value = line.substr(position_in_line_,line.size()-position_in_line_);
			position_in_line_ += value.size()+1;
			return value;
		} else {
			//if value to read is not supposed to be the last value, an error occured
			string line_num;
			std::stringstream out;
			out << line_number;
			line_num = out.str();
			throw UnsupportedOperationException("Syntax error in line "+line_num+" of robot file.");
		}
	}

	value = line.substr(position_in_line_,pos_of_next_seperator-position_in_line_);
	position_in_line_ += value.size()+1;

	//Just to be sure: get rid of leading and trailing spaces
	//(if the input file is correct, then value doesn't contain any leading or trailing spaces)
	boost::trim(value);

	//remove leading and trailing quotes, if value is quoted
	value = remove_quotes(value);

	return value;
}

string Parser::remove_quotes(const string& value) {

	string return_value = value;

	//get first and last character of given string
	char first_char = value.at(0);
	char last_char = value.at(value.length()-1);

	if(first_char == '\"' && last_char == '\"') {
		//first and last characters are quotes => remove quotes
		return_value = value.substr(1,value.length()-2);
	}

	return return_value;
}

double Parser::get_next_double_value_in_line(const string& line, int line_number, bool last_value) {
	//get next value (as string) from given line
	string next_value = get_next_value_in_line(line, line_number, last_value);

	//cast value to double value
	double next_double_value = string_to_double(next_value);

	return next_double_value;
}

Vector3d Parser::get_next_vector3d_in_line(const string& line, int line_number, bool last_value) {

	//TODO(martinah) maybe use arrays here

	//get next three values (as string) from given line
	string value_1 = get_next_value_in_line(line, line_number, false);
	string value_2 = get_next_value_in_line(line, line_number, false);
	string value_3 = get_next_value_in_line(line, line_number, last_value);

	//cast values to double values
	double double_value_1 = string_to_double(value_1);
	double double_value_2 = string_to_double(value_2);
	double double_value_3 = string_to_double(value_3);

	//create Vector3d with from these three values
	Vector3d my_vector;
	my_vector.insert_element(kXCoord, double_value_1);
	my_vector.insert_element(kYCoord, double_value_2);
	my_vector.insert_element(kZCoord, double_value_3);

	return my_vector;
}

double Parser::string_to_double(const string& my_string) {
	//cast given string value to double
	try {
		return boost::lexical_cast<double>(my_string);
	} catch(const boost::bad_lexical_cast& ) {
		throw UnsupportedOperationException("Failed casting string "+my_string+" to double.");
	}
}

void Parser::init_robot_values_for_line(const string& line, int line_number) {

	//begin at beginning of line
	position_in_line_ = 0;

	//The order of these initializations is important!
	double id = get_next_double_value_in_line(line, line_number, false);
	Vector3d position = get_next_vector3d_in_line(line, line_number, false);
	string type = get_next_value_in_line(line, line_number, false);
	Vector3d velocity = get_next_vector3d_in_line(line, line_number, false);
	Vector3d acceleration = get_next_vector3d_in_line(line, line_number, false);
	string status = get_next_value_in_line(line, line_number, false);
	string marker_info = get_next_value_in_line(line, line_number, false);
	string algorithm = get_next_value_in_line(line, line_number, false);
	string color = get_next_value_in_line(line, line_number, false);
	Vector3d x_axis = get_next_vector3d_in_line(line, line_number, false);
	Vector3d y_axis = get_next_vector3d_in_line(line, line_number, false);
	Vector3d z_axis = get_next_vector3d_in_line(line, line_number, true);

	//if no exception is thrown up to this point, values read correctly
	//=> add values to global variables
	initiale_robot_positions_.push_back(position);
	initiale_robot_velocities_.push_back(velocity);
	initiale_robot_accelerations_.push_back(acceleration);
	initiale_robot_types_.push_back(type);
	initiale_robot_stati_.push_back(status);
	initiale_robot_marker_information_.push_back(marker_info);
	initiale_robot_algorithms_.push_back(algorithm);
	initiale_robot_colors_.push_back(color);

	boost::tuple<Vector3d, Vector3d, Vector3d> tmp(x_axis, y_axis, z_axis);
	initiale_robot_coordinate_sytems_.push_back(tmp);
}

void Parser::init_obstacle_values_for_line(const string& line, int line_number) {

	//begin at beginning of line
	position_in_line_ = 0;

	//The order of these initializations is important!
	string type = get_next_value_in_line(line, line_number, false);
	Vector3d position = get_next_vector3d_in_line(line, line_number, false);
	string marker_info = get_next_value_in_line(line, line_number, false);

	//if object is a sphere or a marker, size will equal to the zero-vector,
	//otherwise the values of this vector will be set later.
	Vector3d size;
	size.insert_element(kXCoord, 0);
	size.insert_element(kYCoord, 0);
	size.insert_element(kZCoord, 0);

	//if object is a box or a  marker, radius will equal to zero,
	//otherwise the radius will be set later.
	double radius = 0;

	//depending on type get either radius or width, height and depth
	if(type.compare("box") == 0) {
		//get last three values from line
		size = get_next_vector3d_in_line(line, line_number, true);
	} else if(type.compare("sphere") == 0) {
		//get only next value, which denotes the radius the sphere
		radius = get_next_double_value_in_line(line, line_number, false);
	}

	//if no exception is thrown up to this point, values read correctly
	//=> add values to global variables
	initiale_obstacle_types_.push_back(type);
	initiale_obstacle_positions_.push_back(position);
	initiale_obstacle_marker_information_.push_back(marker_info);
	initiale_obstacle_radius_.push_back(radius);
	initiale_obstacle_size_.push_back(size);
}

void Parser::load_robot_file() {
	load_robot_or_obstacle_file(true);
}

void Parser::load_obstacle_file() {
	load_robot_or_obstacle_file(false);
}

void Parser::load_robot_or_obstacle_file(bool load_robot_file) {
	string line;
	ifstream project_file;
	string filename = robot_filename_;

	//depending on which file to load, specify file extension
	if(load_robot_file)
		filename += ".robot";
	else
		filename += ".obstacle";

	int line_number = 0;

	project_file.open(filename.c_str());
	if(project_file.is_open()) {

		//read first line of file (only contains header not used here)
		if(!project_file.eof()) {
			getline(project_file, line);
			line_number++;
		}

		//read whole file line by line
		while(!project_file.eof()) {
			getline(project_file, line);

			//check if line is not empty
			if(!line.empty()) {

				//check which type of file to load
				if(load_robot_file) {
					//initialize values of robot of this line
					init_robot_values_for_line(line, ++line_number);
				} else {
					//initialize values of obstacle of this line
					init_obstacle_values_for_line(line, ++line_number);
				}
			}
		}
		project_file.close();

	} else {
		throw UnsupportedOperationException("Unable to open file "+filename+".");
	}
}

void Parser::load_projectfiles(const string& project_filename) {
	load_main_project_file(project_filename);
	load_robot_file();
	load_obstacle_file();
}

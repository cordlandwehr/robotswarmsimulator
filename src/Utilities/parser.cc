#include "parser.h"

#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

// some default values are set, especially for output
// please cf. User's Guide
Parser::Parser() :  robot_filename_("rssfile"),
					obstacle_filename_("rssfile") {
	//initialize Parser with default values
	init();

}

Parser::~Parser() {

}

void Parser::init() {
	//define default values
	parameter_map_["COMPASS_MODEL"] = "FULL_COMPASS";
	parameter_map_["ASG"] = "SYNCHRONOUS";
	parameter_map_["STATISTICS_TEMPLATE"] = "ALL";
	parameter_map_["STATISTICS_SUBSETS"] = "{ALL}";

	// no default request handler. Is this good?
}

bool Parser::is_comment(const string& line) {
	return line.empty() || line.at(0)=='#';
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

	//variable has a default value
	if(!var.compare("ROBOT_FILENAME")) {
		return project_filename_;
	}
	else if (!var.compare("OBSTACLE_FILENAME")) {
		return project_filename_;
	}
	else {
		return "NO_DEFAULT_VALUE";
	}
}

string Parser::get_var_value(const string& line, const string& name) {
	size_t pos_of_first_quote_sign = line.find_first_of("\"");
	size_t pos_of_last_quote_sign = line.find_last_of("\"");

	//check if value is correctly quoted and value exists
	if(pos_of_first_quote_sign == string::npos || pos_of_last_quote_sign == string::npos) {
		//get default value, if one exists
		string default_value = get_default_value(name);
		if(default_value.compare("NO_DEFAULT_VALUE")) {
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
		if( default_value.compare("NO_DEFAULT_VALUE") ) {
			//variable has a default value
			return default_value;
		}
		else {
			//variable hasn't a default value and hasn't been initialized
			throw UnsupportedOperationException("Variable according to "+var_name+" has not been initialized and"
					" no default value for this variable exists.");
		}
	} else {
		//return value of var_name
		return variables_and_values.find(var_name)->second;
	}
}

unsigned int Parser::get_uint_value_from_map(map<string,string> variables_and_values,
													const string& var_name) {
	//get string value from map
	string string_value = get_string_value_from_map(variables_and_values, var_name);

	//convert string value to int
	try {
		return boost::lexical_cast<unsigned int>(string_value);

	} catch(const boost::bad_lexical_cast& ) {
		throw UnsupportedOperationException("Failed casting string to integer.");
	}
}

double Parser::get_double_value_from_map(map<string,string> variables_and_values,
													const string& var_name) {
	//get string value from map
	string string_value = get_string_value_from_map(variables_and_values, var_name);

	//convert string value to double
	try {
		return boost::lexical_cast<double>(string_value);

	} catch(const boost::bad_lexical_cast& ) {
		throw UnsupportedOperationException("Failed casting string to double.");
	}
}

std::vector<string> Parser::get_vector_from_map(map<string,string> variables_and_values,
													const string& var_name) {
	//get string value from map
	string string_value = get_string_value_from_map(variables_and_values, var_name);

	std::vector<string> return_vector;

	//check if there exists a value
	if(!string_value.empty()) {

		//split string_value
		string splitted;
		size_t pos_of_last_colon = -1;
		size_t pos_of_next_colon = string_value.find_first_of("\"");

		while(pos_of_next_colon != string::npos) {
			//get substring from last to next colon
			//TODO check if splitting is correct
			splitted = string_value.substr(pos_of_last_colon+1, pos_of_next_colon-pos_of_last_colon-1);
			return_vector.push_back(splitted);

			//reset colon positions
			pos_of_last_colon = pos_of_next_colon;
			pos_of_next_colon = string_value.find_first_of("\"");
		}

		//get last value from string_value (value from last colon to end of string)
		//TODO check if splitting is correct
		splitted = string_value.substr(pos_of_last_colon+1, string_value.size()-pos_of_last_colon);

		return_vector.push_back(splitted);
	}

	return return_vector;
}

std::vector<string> Parser::split_string_by_string(const string& my_string, const string& sep, int max_num_pieces) {
	int pos_of_last_sep = -sep.size();
	int pos_of_next_sep = my_string.find(sep);
	int num_of_pieces = 1;
	string splitted_piece;
	std::vector<string> splitted_string;

	//TODO(martinah) maybe add exception handling
	while(pos_of_next_sep != string::npos && num_of_pieces < max_num_pieces) {
		splitted_piece = my_string.substr(pos_of_last_sep+sep.size(), pos_of_next_sep-(pos_of_last_sep+sep.size()));
		boost::trim(splitted_piece);
		splitted_string.push_back(splitted_piece);
		num_of_pieces++;

		//set new seperator positions
		pos_of_last_sep = pos_of_next_sep;
		pos_of_next_sep = my_string.find(sep, pos_of_last_sep+sep.size());
	}
	//split last piece
	splitted_piece = my_string.substr(pos_of_last_sep+sep.size());
	boost::trim(splitted_piece);
	splitted_string.push_back(splitted_piece);
	return splitted_string;
}


void Parser::init_variables(map<string,string> variables_and_values) {

	//Variable names saved in the map are specified in the "Projectfiles Specification"-document
	// if ".obstacle" exists at end of filename: erase it!
	obstacle_filename_ = get_string_value_from_map(variables_and_values, "OBSTACLE_FILENAME");
	if (obstacle_filename_.rfind(".obstacle")!=string::npos)
		obstacle_filename_.erase (obstacle_filename_.rfind(".obstacle"),9);
	// if ".robot" exists at end of filename: erase it!
	robot_filename_ = get_string_value_from_map(variables_and_values, "ROBOT_FILENAME");
	if (robot_filename_.rfind(".robot")!=string::npos)
		robot_filename_.erase (robot_filename_.rfind(".robot"),6);

}

void Parser::load_main_project_file(const string& project_filename) {
	string line;
	ifstream project_file;
	project_filename_ = project_filename;
	string main_project_filename = project_filename_ + ".swarm";


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
				parameter_map_.insert(pair<string,string>(var_name,var_value));
			}
		}
		project_file.close();

		//up to now: read all variables and its values from project file and
		//inserted them into a map

		//initialize variables
		init_variables(parameter_map_);

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
	value = remove_quotes_and_leading_and_trailing_spaces(value);

	return value;
}

string Parser::remove_quotes_and_leading_and_trailing_spaces(const string& value) {

	//get rid of leading and trailing spaces
	string return_value = value;
	boost::trim(return_value);

	//get first and last character of given string
	char first_char = value.at(0);
	char last_char = value.at(value.length()-1);

	if(first_char == '\"' && last_char == '\"') {
		//first and last characters are quotes => remove quotes
		return_value = value.substr(1,value.length()-2);
	}

	//get rid of leading and trailing spaces
	boost::trim(return_value);

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
	// TODO(peter) is there a reason that this variable is read but never used? (see compiler warning)
	//TODO(martinah) No, there is no real reason for this. I added this value to each robot, cause maybe it will be used
	//in the future, but right now, it isn't. And I just read this variable to increase the value position_in_line_ correct.
	//Sure, I could have done this without saving the id.
	//TODO(kurras) funny discussion :) shouldn't we simply remove the "double id = " then ?
	//...or
	//...generate more LOC ? ;P
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
	initiale_robot_coordinate_systems_.push_back(tmp);
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
	string filename = "";

	// todo(peter) It seems that these file names are absolute. But they should be relative to the directory of the main
	//             projectfile (.swarm), because they're referenced there.
	//depending on which file to load, specify file extension
	if(load_robot_file)
		filename += robot_filename_ + ".robot";
	else
		filename += obstacle_filename_ + ".obstacle";

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

void Parser::save_main_project_file(const string& project_filename) {
	ofstream project_file;
	project_file.open((project_filename+".swarm").c_str());
	if(project_file.is_open()) {
		// Save the map. Should contain all variables.
		project_file << "ROBOT_FILENAME=\"" << robot_filename_ << "\"" << endl;
		project_file << "OBSTACLE_FILENAME=\"" << obstacle_filename_ << "\"" << endl;

		std::map<string, string>::iterator param_map_iter = parameter_map_.begin();
		while(param_map_iter != parameter_map_.end()) {
			project_file << (*param_map_iter).first << "=" << "\"" << (*param_map_iter).second << "\"" << std::endl;
			param_map_iter++;
		}
		project_file.close();
	} else {
		throw UnsupportedOperationException("Unable to open project file" +project_filename+"!");
	}
}

void Parser::save_robot_file(const WorldInformation& world_info) {

	ofstream robot_file;
	robot_file.open((robot_filename_+".robot").c_str());

	if(robot_file.is_open()) {
		//write robot header
		robot_file << "\"ID\",";
		robot_file << "\"x-position\",\"y-position\",\"z-position\",";
		robot_file << "\"type\",";
		robot_file << "\"x-velocity\",\"y-velocity\",\"z-velocity\",";
		robot_file << "\"x-acceleration\",\"y-acceleration\",\"z-acceleration\",";
		robot_file << "\"status\",";
		robot_file << "\"marker-info\",";
		robot_file << "\"algorithm\",";
		robot_file << "\"color\",";
		robot_file << "\"x-axis-1\",\"x-axis-2\",\"x-axis-3\",";
		robot_file << "\"y-axis-1\",\"y-axis-2\",\"y-axis-3\",";
		robot_file << "\"z-axis-1\",\"z-axis-2\",\"z-axis-3\"" << endl;

		//getting all RobotData from current world-information
	//	std::vector< boost::shared_ptr<RobotData> > robots_data = sim_kernel_->history()->get_newest().robot_data();
		std::vector< boost::shared_ptr<RobotData> > robots_data = world_info.robot_data();

		//iterate over all RobotData to parse them Robot by Robot
		for (vector<boost::shared_ptr<RobotData> >::iterator it = robots_data.begin();
				it!=robots_data.end(); ++it) {
			robot_file << write_robot(*it);
		}

		robot_file.close();
	} else {
		throw UnsupportedOperationException("Unable to open robot file" +robot_filename_+"!");
	}
}

string Parser::write_robot(boost::shared_ptr<RobotData> robot_data) {

	stringstream output;

	output << robot_data->id()->id() << ",";

	output << robot_data->position()(kXCoord) << ",";
	output << robot_data->position()(kYCoord) << ",";
	output << robot_data->position()(kZCoord) << ",";

	output << robot_data->type() << ",";

	output << robot_data->velocity()(kXCoord) << ",";
	output << robot_data->velocity()(kYCoord) << ",";
	output << robot_data->velocity()(kZCoord) << ",";

	output << robot_data->acceleration()(kXCoord) << ",";
	output << robot_data->acceleration()(kYCoord) << ",";
	output << robot_data->acceleration()(kZCoord) << ",";

	output << robot_data->status() << ",";

	//TODO(mmarcus) include marker-information
	output << "0,";
	output << "\"" << robot_data->robot().get_algorithm_id() << "\",";
	//TODO(mmarcus) include color-information
	output << "0,";

	//x-axis
	output << (*(boost::get<0>(robot_data->coordinate_system_axis())))(kXCoord) << ",";
	output << (*(boost::get<0>(robot_data->coordinate_system_axis())))(kYCoord) << ",";
	output << (*(boost::get<0>(robot_data->coordinate_system_axis())))(kZCoord) << ",";

	//y-axis
	output << (*(boost::get<1>(robot_data->coordinate_system_axis())))(kXCoord) << ",";
	output << (*(boost::get<1>(robot_data->coordinate_system_axis())))(kYCoord) << ",";
	output << (*(boost::get<1>(robot_data->coordinate_system_axis())))(kZCoord) << ",";

	//z-axis
	output << (*(boost::get<2>(robot_data->coordinate_system_axis())))(kXCoord) << ",";
	output << (*(boost::get<2>(robot_data->coordinate_system_axis())))(kYCoord) << ",";
	output << (*(boost::get<2>(robot_data->coordinate_system_axis())))(kZCoord) << endl;

	return output.str();

	}

void Parser::save_obstacle_file(const WorldInformation& world_info) {

	ofstream obstacle_file;
	obstacle_file.open((obstacle_filename_+".obstacle").c_str());

	if(obstacle_file.is_open()) {
		//write obstacle header
		obstacle_file << "\"type\",";
		obstacle_file << "\"x-position\",";
		obstacle_file << "\"y-position\",";
		obstacle_file << "\"z-position\",";
		obstacle_file << "\"marker-info\",";
		obstacle_file << "\"size-info\",";
		obstacle_file << "\"\",";
		obstacle_file << "\"\"" << endl;

		//fetching obstacles from the actual world-information through history-reference
		vector<boost::shared_ptr<Obstacle> > obstacles = world_info.obstacles();

		//iterate over all obstacles to parse them obstacle by obstacle
		for (vector<boost::shared_ptr<Obstacle> >::iterator it = obstacles.begin(); it!=obstacles.end(); ++it) {
			obstacle_file << write_obstacle(*it);
		}

		//fetching marker-informations from actual world-information through history-reference
		vector<boost::shared_ptr<WorldObject> > markers = world_info.markers();

		//iterate over all markers
		for (vector<boost::shared_ptr<WorldObject> >::iterator it = markers.begin(); it!=markers.end(); ++it) {
			obstacle_file << write_marker(*it);
		}

		obstacle_file.close();
	} else {
		throw UnsupportedOperationException("Unable to open obstacle file" +obstacle_filename_+"!");
	}
}

string Parser::write_obstacle(boost::shared_ptr<Obstacle> current_obstacle) {

	stringstream output;

	//check by dynamic typecast if the obstacle is a sphere or a box.
	//See event_handler.cc for details to this technique
	if(boost::shared_ptr<Box> box_obstacle = boost::dynamic_pointer_cast<Box>(current_obstacle)) {
		output << "\"box\",";

		output << box_obstacle->position()(kXCoord) << ",";
		output << box_obstacle->position()(kYCoord) << ",";
		output << box_obstacle->position()(kZCoord) << ",";

		//TODO(mmarcus) include marker-information
		output << box_obstacle->width() << ",";
		output << box_obstacle->height() << ",";
		output << box_obstacle->depth() << endl;
	} else if(boost::shared_ptr<Sphere> sphere_obstacle = boost::dynamic_pointer_cast<Sphere>(current_obstacle)) {
		output << "\"sphere\",";

		output << sphere_obstacle->position()(kXCoord) << ",";
		output << sphere_obstacle->position()(kYCoord) << ",";
		output << sphere_obstacle->position()(kZCoord) << ",";

		//TODO(mmarcus) include marker-information
		output << sphere_obstacle->radius() << ",\"\",\"\"" << endl;
	} else {
		throw UnsupportedOperationException("Illegal type of obstacle found.");
	}

	return output.str();
}

string Parser::write_marker(boost::shared_ptr<WorldObject> marker) {

	stringstream output;

	output << "\"marker\",";

	output << marker->position()(kXCoord) << ",";
	output << marker->position()(kYCoord) << ",";
	output << marker->position()(kZCoord) << ",";
	//TODO(mmarcus) include marker-information
	output << "\"\",\"\",\"\"" << endl;

	return output.str();
}

void Parser::save_projectfiles(const string& project_filename, const WorldInformation& world_info) {
	save_main_project_file(project_filename);
	save_robot_file(world_info);
	save_obstacle_file(world_info);
}



/*** SET-methods for main projectfile variables ***/
void Parser::set_obstacle_filename(const string& obstacle_filename) {
	obstacle_filename_ = obstacle_filename;
}

void Parser::set_robot_filename(const string& robot_filename) {
	robot_filename_ = robot_filename;
}


void Parser::set_project_filename(const string& project_filename) {
	project_filename_ = project_filename;
}



/*** GET-methods for main projectfile variables ***/
const string& Parser::robot_filename() const {
	return robot_filename_;
}

/*** GET-methods for robot data ***/
vector<Vector3d>& Parser::robot_positions() {
	return initiale_robot_positions_;
}
vector<Vector3d>& Parser::robot_velocities() {
	return initiale_robot_velocities_;
}

vector<Vector3d>& Parser::robot_accelerations() {
	return initiale_robot_accelerations_;
}
vector<string>& Parser::robot_types() {
	return initiale_robot_types_;
}
vector<string>& Parser::robot_stati() {
	return initiale_robot_stati_;
}
vector<string>& Parser::robot_marker_information() {
	return initiale_robot_marker_information_;
}
vector<string>& Parser::robot_algorithms() {
	return initiale_robot_algorithms_;
}
vector<string>& Parser::robot_colors() {
	return initiale_robot_colors_;
}
vector<boost::tuple<Vector3d, Vector3d, Vector3d > >& Parser::robot_coordinate_systems() {
	return initiale_robot_coordinate_systems_;
}


/*** GET-methods for obstacle data ***/
vector<string>& Parser::obstacle_types() {
	return initiale_obstacle_types_;
}
vector<Vector3d>& Parser::obstacle_positions() {
	return initiale_obstacle_positions_;
}
vector<string>& Parser::obstacle_marker_information() {
	return initiale_obstacle_marker_information_;
}
vector<double>& Parser::obstacle_radius() {
	return initiale_obstacle_radius_;
}
vector<Vector3d>& Parser::obstacle_size() {
	return initiale_obstacle_size_;
}

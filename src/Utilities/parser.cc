/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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

#include "parser.h"

#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/options_description.hpp>

//include for enums of RobotType and RobotStatus
#include "../Model/robot_data.h"

using namespace std;


// some default values are set, especially for output
// please cf. User's Guide
Parser::Parser() :  robot_filename_("rssfile"),
					dumpnumber_(0) {
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
	parameter_map_["ROBOT_CONTROL"] = "UNIFORM_ROBOT_CONTROL";
	parameter_map_["CAMERA_POSITION"] = "0,0,0";
	parameter_map_["CAMERA_DIRECTION"] = "1,0,0";
	parameter_map_["CAMERA_TYPE"] = "FOLLOW";
}

bool Parser::is_comment(const string& line) {
	return line.empty() || line.at(0)=='#';
}

bool Parser::contains_assignment(const string& line) {
	const std::size_t pos_of_equal_sign = line.find_first_of("=");
	return pos_of_equal_sign != string::npos;
}

string Parser::get_var_name(const string& line) {
	const std::size_t pos_of_equal_sign = line.find_first_of("=");

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
	} else {
		return "NO_DEFAULT_VALUE";
	}
}

string Parser::get_var_value(const string& line, const string& name) {
	size_t pos_of_first_quote_sign = line.find_first_of("=") + 1; //line.find_first_of("\"");
	size_t pos_of_last_quote_sign = line.length(); // line.find_last_of("\"");

	//check if value is correctly quoted and value exists
	if(pos_of_first_quote_sign == string::npos || pos_of_last_quote_sign == string::npos) {
		//get default value, if one exists
		string default_value = get_default_value(name);
		if(default_value.compare("NO_DEFAULT_VALUE")) {
			return default_value;
		}
		else {
			throw UnsupportedOperationException("Syntax error in main project file. Either variable value in line "
				"\""+line+"\" is not quite correct or value is not defined.");
		}
	}

	return line.substr(pos_of_first_quote_sign/*+1*/,
			           pos_of_last_quote_sign-pos_of_first_quote_sign/*-1*/);

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

void Parser::init_variables(map<string,string> variables_and_values) {

	//Variable names saved in the map are specified in the "Projectfiles Specification"-document
	// if ".obstacle" exists at end of filename: erase it!
	// if ".robot" exists at end of filename: erase it!
	robot_filename_ = get_string_value_from_map(variables_and_values, "ROBOT_FILENAME");
	if (robot_filename_.rfind(".robot")!=string::npos)
		robot_filename_.erase (robot_filename_.rfind(".robot"),6);

	string temp_world_modifiers = get_string_value_from_map(variables_and_values, "WORLD_MODIFIERS");

//	boost::is_any_of(",");

	std::vector<std::string> temp_split_world_modifiers;

	boost::split(temp_split_world_modifiers, temp_world_modifiers, boost::is_any_of(","));
    
    using boost::filesystem::path;

	for(std::size_t i = 0; i < temp_split_world_modifiers.size(); i++){
		std::string temp_string = temp_split_world_modifiers[i]; //TODO: Is there a memory leak?
		boost::trim(temp_string);
        
        if (temp_string.rfind(".lua") == temp_string.size()-4) {
            // robot file is located relatively to main project file
            path modifier_file = path(project_filename_).parent_path() / robot_filename_;
            temp_string = (modifier_file.parent_path() / temp_string).string();
        }
        
		world_modifiers_.push_back(temp_string);
	}
}

void Parser::load_main_project_file(const string& project_filename) {
	
	project_filename_ = project_filename;
	string main_project_filename = project_filename_ + ".swarm";
		
	boost::program_options::options_description desc("Main project file options");
	desc.add_options()
    ("PROJECT_NAME", boost::program_options::value<string>(), "set project name")
	("ROBOT_FILENAME", boost::program_options::value<string>(), "set robot file name")
	("WORLD_MODIFIERS", boost::program_options::value< vector<string> >(), "set world modifiers")	
	("ASG", boost::program_options::value<string>()->default_value("SYNCHRONOUS_WM"), "set activation sequence generator")
	("VIEW", boost::program_options::value<string>()->default_value("LOCAL_GRAPH_VIEW"), "set view to use")
	;
	

	std::ifstream in( main_project_filename.c_str() ); //asetzer: workaround, see: http://lists.boost.org/boost-users/2005/05/11740.php
	boost::program_options::store(boost::program_options::parse_config_file(in, desc), parameter_map_boost_);
	boost::program_options::notify(parameter_map_boost_);
	

	string line;
	std::ifstream project_file;
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
				parameter_map_[var_name] = var_value;
			}
		}
		project_file.close();

		//up to now: read all variables and its values from project file and
		//inserted them into a map

		//initialize variables
		init_variables(parameter_map_);

	} else {
		throw UnsupportedOperationException("Unable to open project file: "+main_project_filename);
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

	string return_string = remove_quotes_and_leading_and_trailing_spaces(my_string);

	try {
		//cast given string value to double
		return boost::lexical_cast<double>(return_string);
	} catch(const boost::bad_lexical_cast& ) {
		throw UnsupportedOperationException("Failed casting string "+return_string+" to double.");
	}
}

void Parser::init_robot_values_for_line(const string& line, int line_number) {

	//continue with next line if this is a comment-line, eg. begins with '#'
	if (line.substr(0,1).compare("#")==0)
		return;

	//begin at beginning of line
	position_in_line_ = 0;

	//The order of these initializations is important!
	get_next_double_value_in_line(line, line_number, false); // value not needed atm
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

	// if algorithm has suffix '.lua', it is a filename that is to be interpreted relatively to the robot project file
	using boost::filesystem::path;
	if (algorithm.rfind(".lua") == algorithm.size()-4) {
		// robot file is located relatively to main project file
		path robot_file = path(project_filename_).parent_path() / robot_filename_;
		algorithm = (robot_file.parent_path() / algorithm).string();
	}

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


void Parser::load_robot_file() {
	load_robot_or_obstacle_file(true);
}

void Parser::load_robot_or_obstacle_file(bool load_robot_file) {
	string line;
	boost::filesystem::ifstream project_file;

	//depending on which file to load, specify file extension
	string filename;
	if(load_robot_file) {
		// this allows ending ".csv" for robot files.
		if (robot_filename_.rfind(".csv")!=string::npos)
			filename = robot_filename_;
		else
			filename = robot_filename_ + ".robot";
	}
	// the robot/obstacle filenames are interpreted relatively to the location of the main project file
	using boost::filesystem::path;
	path file = path(project_filename_).parent_path() / filename;

	int line_number = 0;

	project_file.open(file);
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
				}
			}
		}
		project_file.close();

	} else {
		throw UnsupportedOperationException("Unable to open file: " + file.string() + ".");
	}
}

void Parser::load_projectfiles(const string& project_filename) {
	load_main_project_file(project_filename);
	load_robot_file();
}

void Parser::save_main_project_file(const string& project_filename) {
	ofstream project_file;
	project_file.open((project_filename + ".swarm").c_str());
	if(project_file.is_open()) {
		// Save the map. Should contain all variables.
		project_file << "ROBOT_FILENAME=\"" << robot_filename_ << "\"" << endl;

		std::map<string, string>::iterator param_map_iter = parameter_map_.begin();
		while(param_map_iter != parameter_map_.end()) {
			if ((*param_map_iter).first == "ROBOT_FILENAME" || (*param_map_iter).first == "OBSTACLE_FILENAME") {
				project_file << "# Old robot/obstacle-files: ";
			}
			project_file << (*param_map_iter).first << "=" << "\"" << (*param_map_iter).second << "\"" << std::endl;
			param_map_iter++;
		}
		project_file.close();
		dumpnumber_++;
	} else {
		throw UnsupportedOperationException("Unable to open project file: " +project_filename);
	}
}

void Parser::save_robot_file(const WorldInformation& world_info) {
	// the robot/obstacle filenames are interpreted relatively to the location of the main project file
	using boost::filesystem::path;
	path file = path(project_filename_).parent_path() / (robot_filename_ + ".robot");

	boost::filesystem::ofstream robot_file;
	robot_file.open(file);

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
		throw UnsupportedOperationException("Unable to open robot file: " + file.string() + "!");
	}
}

string Parser::write_robot(boost::shared_ptr<RobotData> robot_data) {

	stringstream output;

	output << robot_data->id()->id() << ",";

	output << robot_data->position()(kXCoord) << ",";
	output << robot_data->position()(kYCoord) << ",";
	output << robot_data->position()(kZCoord) << ",";

	if ( robot_data->type() == MASTER ) {
		output << "\"MASTER\"" << ",";
	} else if ( robot_data->type() == SLAVE ){
		output << "\"SLAVE\"" << ",";
	} else {
		throw UnsupportedOperationException("Parser found some robottype that it cannot write.");
	}

	output << robot_data->velocity()(kXCoord) << ",";
	output << robot_data->velocity()(kYCoord) << ",";
	output << robot_data->velocity()(kZCoord) << ",";

	output << robot_data->acceleration()(kXCoord) << ",";
	output << robot_data->acceleration()(kYCoord) << ",";
	output << robot_data->acceleration()(kZCoord) << ",";

	if ( robot_data->status() == SLEEPING ) {
		output << "\"SLEEPING\"" << ",";
	} else if ( robot_data->status() == READY) {
		output << "\"READY\"" << ",";
	} else {
		throw UnsupportedOperationException("Parser found some robotstatus that it cannot write.");
	}

	//TODO(mmarcus) include marker-information
	output << "0,";

	output << "\"" << robot_data->robot().get_algorithm_id() << "\",";

	output << robot_data->color() << ",";

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


void Parser::save_projectfiles(const string& project_filename, const WorldInformation& world_info) {
	save_main_project_file(project_filename);
	save_robot_file(world_info);
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

/*** GET-method for world modfiers ***/
vector<string>& Parser::world_modifiers() {
	return world_modifiers_;
}


int Parser::dumpnumber() {
	return dumpnumber_;
}


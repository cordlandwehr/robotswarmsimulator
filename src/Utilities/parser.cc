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
#include "console_output.h"

using namespace std;


// some default values are set, especially for output
// please cf. User's Guide
Parser::Parser() :  robot_filename_("rssfile"),
					dumpnumber_(0) { }

Parser::~Parser() {

}

std::map<std::string, std::string>& Parser::parameter_map() {
  //this might be called from classes like SzenarioGenerator which are no longer used
	ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::error) << "Accessing deprecated parameter_map (this should not happen, only from deprecated classes)" << endl;
	return parameter_map_; 
}


string Parser::get_string_value_from_map(const boost::program_options::variables_map& variables_and_values,
													const string& var_name) {
	//get iterator to value to be searched for
	if (parameter_map_boost_.count(var_name)) {
	  return parameter_map_boost_[var_name].as<std::string>();
	} else {
		//variable hasn't a default value and hasn't been initialized
		throw UnsupportedOperationException("Variable according to "+var_name+" has not been initialized and"
			" no default value for this variable exists.");	  
	}
	
	/*map<string,string>::const_iterator map_iterator = variables_and_values.find(var_name);

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
	}*/
}

void Parser::init_variables() {

	//Variable names saved in the map are specified in the "Projectfiles Specification"-document
	// if ".obstacle" exists at end of filename: erase it!
	// if ".robot" exists at end of filename: erase it!
	robot_filename_ = parameter_map_boost()["ROBOT_FILENAME"].as<string>();
	
	if (robot_filename_.rfind(".robot")!=string::npos)
		robot_filename_.erase (robot_filename_.rfind(".robot"),6);
	
	edge_filename_ = parameter_map_boost()["EDGE_FILENAME"].as<string>();

	std::vector<string> temp_split_world_modifiers = parameter_map_boost()["WORLD_MODIFIERS"].as<std::vector<string> >();
   
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
	("EDGE_FILENAME", boost::program_options::value<string>()->default_value(""), "set edge file name")
	("WORLD_MODIFIERS", boost::program_options::value< vector<string> >(), "set world modifiers")	
	("ASG", boost::program_options::value<string>()->default_value("SYNCHRONOUS_WM"), "set activation sequence generator")
	("VIEW", boost::program_options::value<string>()->default_value("LOCAL_GRAPH_VIEW"), "set view to use")
	;
	

	
	try {
	  std::ifstream in( main_project_filename.c_str() ); //asetzer: workaround, see: http://lists.boost.org/boost-users/2005/05/11740.php
	  
      if (!in) {
		ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::error) << "The specified project file has not been found!" << std::endl;	
		
		//TODO (Sascha?) Entry point for "file not found" handling
		
		throw;
	  }
	  
	  boost::program_options::store(boost::program_options::parse_config_file(in, desc), parameter_map_boost_);
	  boost::program_options::notify(parameter_map_boost_);	  
	}
	catch (std::exception& e) {
		ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::error) << e.what() << std::endl;
		throw;
	}
	catch(...) {
		ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::error) <<  "Uncaught unknown exception." << std::endl;
		throw; //rethrow exception
	}	

	ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::debug) << "Finished parsing the project file, setting up variables." << endl;
	
	//initialize variables
	init_variables();

}

string Parser::get_next_value_in_line(const string& line, int line_number, bool last_value) {
	//character that seperates values in line
	char separator = ',';
	string value;

	size_t pos_of_next_separator = line.find_first_of(separator, position_in_line_);

	//check if there exists a next seperator
	if(pos_of_next_separator == string::npos) {

		if(last_value) {
			//value to read is last value => return end of line beginning at given position
			value = line.substr(position_in_line_,line.size()-position_in_line_);
			position_in_line_ += value.size()+1;
			//Just to be sure: get rid of leading and trailing spaces
			value = remove_quotes_and_leading_and_trailing_spaces(value);
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

	value = line.substr(position_in_line_,pos_of_next_separator-position_in_line_);
	position_in_line_ += value.size()+1;

	//Just to be sure: get rid of leading and trailing spaces
	//(if the input file is correct, then value doesn't contain any leading or trailing spaces) //TODO ???
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
	int id = get_next_double_value_in_line(line, line_number, false);
	Vector3d position = get_next_vector3d_in_line(line, line_number, false);
	string marker_info = "";//get_next_value_in_line(line, line_number, false); //TODO asetzer: see if we ever need this
	string algorithm = get_next_value_in_line(line, line_number, true);

	// if algorithm has suffix '.lua', it is a filename that is to be interpreted relatively to the robot project file
	using boost::filesystem::path;
	if (algorithm.rfind(".lua") == algorithm.size()-4) {
		// robot file is located relatively to main project file
		path robot_file = path(project_filename_).parent_path() / robot_filename_;
		algorithm = (robot_file.parent_path() / algorithm).string();
	}

	//if no exception is thrown up to this point, values read correctly
	//=> add values to global variables
	initiale_robot_ids_.push_back(id);
	initiale_robot_positions_.push_back(position);
	initiale_robot_marker_information_.push_back(marker_info);
	initiale_robot_algorithms_.push_back(algorithm);

}

void Parser::init_edge_values_for_line(const string& line, int line_number) {

	//continue with next line if this is a comment-line, eg. begins with '#'
	if (line.substr(0,1).compare("#")==0)
		return;

	//begin at beginning of line
	position_in_line_ = 0;

	//The order of these initializations is important!
	int end1_ID = get_next_double_value_in_line(line, line_number, false);
	int end2_ID = get_next_double_value_in_line(line, line_number, false);
	int bidirectional = get_next_double_value_in_line(line, line_number, true);	

	initiale_edge_end1_.push_back(end1_ID);
	initiale_edge_end2_.push_back(end2_ID);
	initiale_edge_bidirectional_.push_back(bidirectional);

}

void Parser::load_robot_file() {
	load_robot_or_edge_file(true);
}

void Parser::load_edge_file() {
	load_robot_or_edge_file(false);
}

void Parser::load_robot_or_edge_file(bool load_robot_file) {
	string line;
	boost::filesystem::ifstream project_file;

	//depending on which file to load, specify file extension
	string filename;
	if(load_robot_file) {
	  filename = robot_filename_;
	  
	  // this allows ending ".robot" for robot files.
	  if (filename.rfind(".csv")==string::npos)
		filename = filename + ".robot";
	  
	} else {
	  filename = edge_filename_;
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
				} else {
					init_edge_values_for_line(line, ++line_number);
				}
			}
		}
		project_file.close();

	} else {
	  if (!load_robot_file) {
		  //edge file is missing - print a warning and assume no edges are there
		  ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::error) << "Edge file cannot be found. Assuming there are no edges." << endl;		  
	  }
	  else {
		throw UnsupportedOperationException("Unable to open file: " + file.string() + ".");
	  }
	}
}

void Parser::load_projectfiles(const string& project_filename) {
	load_main_project_file(project_filename);
	load_robot_file();
	if (edge_filename() != "")
	  load_edge_file();
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

const string& Parser::edge_filename() const {
	return edge_filename_;
}

/*** GET-methods for robot data ***/
vector<int>& Parser::robot_ids() {
	return initiale_robot_ids_;
}

vector<Vector3d>& Parser::robot_positions() {
	return initiale_robot_positions_;
}
vector<string>& Parser::robot_marker_information() {
	return initiale_robot_marker_information_;
}
vector<string>& Parser::robot_algorithms() {
	return initiale_robot_algorithms_;
}

vector<int>& Parser::edge_end1() {
	return initiale_edge_end1_;
}
vector<int>& Parser::edge_end2() {
	return initiale_edge_end2_;
}
vector<int>& Parser::edge_bidirectional() {
	return initiale_edge_bidirectional_;
}

/*** GET-method for world modfiers ***/
vector<string>& Parser::world_modifiers() {
	return world_modifiers_;
}


int Parser::dumpnumber() {
	return dumpnumber_;
}


boost::filesystem::path Parser::get_project_path(){
	using boost::filesystem::path;
	return path(project_filename_).parent_path();
}


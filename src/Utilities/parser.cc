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
	statistics_module_ = get_int_value_from_map(variables_and_values, "STATISTICS_MODULE");
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

void Parser::load_robot_file() {
	//TODO implement
}

void Parser::load_obstacle_file() {
	//TODO implement
}

void Parser::load_projectfiles(const string& project_filename) {
	load_main_project_file(project_filename);
	load_robot_file();
	load_obstacle_file();
}

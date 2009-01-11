#include "simulation_kernel.h"
#include "../Utilities/unsupported_operation_exception.h"

SimulationKernel::SimulationKernel() {
	// TODO Auto-generated constructor stub

}

SimulationKernel::~SimulationKernel() {
	// TODO Auto-generated destructor stub
}

const vector<boost::shared_ptr<Robot> >& SimulationKernel::robots() const {
	return robots_;
}

const boost::shared_ptr<History>& SimulationKernel::history() const {
	return history_;
}

void SimulationKernel::init(const string& project_filename) {
	load_projectfiles(project_filename);
	//TODO implement rest of initialization
	//create Robot
	//create Robot-Data and pass a reference to the according robot-object
}

bool SimulationKernel::is_comment(const string& line) {
	return (line.at(0)=='#');
}

bool SimulationKernel::contains_assignment(const string& line) {
	unsigned int pos_of_equal_sign = line.find_first_of("=");

	//check if equal sign exists in given string
	if(pos_of_equal_sign < 0 || pos_of_equal_sign >= line.size())
		return false;
	else
		return true;
}

string SimulationKernel::get_var_name(const string& line) {
	unsigned int pos_of_equal_sign = line.find_first_of("=");
	string var_name = line.substr(0,pos_of_equal_sign);
	//get rid of unwanted leading and trailing spaces
	boost::trim(var_name);
	return var_name;
}

string SimulationKernel::get_var_value(const string& line) {
	unsigned int pos_of_first_quote_sign = line.find_first_of("\"");
	unsigned int pos_of_last_quote_sign = line.find_last_of("\"");

	return line.substr(pos_of_first_quote_sign+1,
			           pos_of_last_quote_sign-pos_of_first_quote_sign-1);

}

string SimulationKernel::get_string_value_from_map(map<string,string> variables_and_values,
													const string& var_name) {
	//get iterator to value to be searched for
	map<string,string>::const_iterator map_iterator = variables_and_values.find(var_name);

	//check if var_name exists in given map
	if(map_iterator == variables_and_values.end()) {
		//TODO(martinah) throw exception
		cout << "Variable according to " << var_name << " has not been initialized." << endl;
		return "";
	} else {
		//return value of var_name
		return variables_and_values.find(var_name)->second;
	}
}

int SimulationKernel::get_int_value_from_map(map<string,string> variables_and_values,
													const string& var_name) {
	//get string value from map
	string string_value = get_string_value_from_map(variables_and_values, var_name);

	//convert string value to int
	try {
		return boost::lexical_cast<int>(string_value);

	} catch(const boost::bad_lexical_cast& ) {
		//TODO(martinah) throw exception
	}
}

void SimulationKernel::init_variables(map<string,string> variables_and_values) {

	//Variable names saved in the map are specified in the "Projectfiles Specification"-document
	asg_= get_int_value_from_map(variables_and_values, "ASG");
	compass_model_ = get_string_value_from_map(variables_and_values, "COMPASS_MODEL");
	event_handler_ = get_int_value_from_map(variables_and_values, "EVENT_HANDLER");
	obstacle_filename_ = get_string_value_from_map(variables_and_values, "OBSTACLE_FILENAME");
	project_name_ = get_string_value_from_map(variables_and_values, "PROJECT_NAME");
	robot_filename_ = get_string_value_from_map(variables_and_values, "ROBOT_FILENAME");
	statistics_module_ = get_int_value_from_map(variables_and_values, "STATISTICS_MODULE");
}

void SimulationKernel::load_main_project_file(const string& project_filename) {
	string line;
	ifstream project_file;
	map<string, string> variables_and_values;

	project_file.open(project_filename.c_str());
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
				var_value = get_var_value(line);

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
		//TODO(martinah) throw according exception
		cout << "Unable to open given project file." << endl;
	}
}

void SimulationKernel::load_robot_file() {
	//TODO implement
}

void SimulationKernel::load_obstacle_file() {
	//TODO implement
}

void SimulationKernel::load_projectfiles(const string& project_filename) {
	load_main_project_file(project_filename);
	load_robot_file();
	load_obstacle_file();
}

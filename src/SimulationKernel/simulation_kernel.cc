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

void SimulationKernel::init(const string& project_filename, boost::shared_ptr<History> history) {
	// set history
	history_ = history;
	load_projectfiles(project_filename);
	//TODO implement rest of initialization
	//2. Create according ViewObject.
	//3. Create according WorldObjects (Robots and RobotData (passing reference
	//     to according robot-objects, Obstacles).
	//4. Create initial WorldInformation.
	//5. Add WorldInformation to History Object.
	//6. Create according ASG.
	//7. Create according EventHandler.
	//8. (Create StatisticObject.)
	//9. Register SimulationObservers (ViewObject, ASG, maybe StatisticObject)
}

void SimulationKernel::step() {
	//TODO(mmarcus) step
}

void SimulationKernel::multistep(int steps) {
	for (int i = 0; i < steps; i++) {
		step();
	}
}

bool SimulationKernel::is_comment(const string& line) {
	return (line.at(0)=='#');
}

bool SimulationKernel::contains_assignment(const string& line) {
	unsigned int pos_of_equal_sign = line.find_first_of("=");

	//TODO (dwonisch): return pos_of_equal_sign != string::npos; ?
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
	return 0;
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

void SimulationKernel::save_main_project_file(const string& project_filename) {

	//TODO(mmarcus) catch exceptions
	// open outputstream
	ofstream project_file;
	project_file.open(project_filename.c_str());

	//write variables
	project_file << "ASG=\"" << asg_ << "\"" << endl;
	project_file << "COMPASS_MODEL=\"" << compass_model_ << "\"" << endl;
	project_file << "EVENT_HANDLER=\"" << event_handler_ << "\"" << endl;
	project_file << "OBSTACLE_FILENAME=\"" << obstacle_filename_ << "\"" << endl;
	project_file << "PROJECT_NAME=\"" << project_name_ << "\"" << endl;
	project_file << "ROBOT_FILENAME=\"" << robot_filename_ << "\"" << endl;
	project_file << "STATISTICS_MODULE=\"" << statistics_module_ << "\"" << endl;

	project_file.close();

}

void SimulationKernel::save_robot_file() {
	//TODO(mmarcus) implement
}

void SimulationKernel::save_obstacle_file() {

	ofstream obstacle_file;
	//TODO(mmarcus) this will not work - path needs to be specified
	obstacle_file.open(obstacle_filename().c_str());

	//TODO(mmarcus) get obstacle-vector from current world_information and iterate
	// write_obstacle(...);

	obstacle_file.close();

}

void SimulationKernel::save_projectfiles(const string& project_filename) {
	save_main_project_file(project_filename);
	save_robot_file();
	save_obstacle_file();
}

void SimulationKernel::write_obstacle_header(ofstream obstacle_file) {
	obstacle_file << "\"type\",";
	obstacle_file << "\"x-position\",";
	obstacle_file << "\"y-position\",";
	obstacle_file << "\"z-position\",";
	obstacle_file << "\"marker-info\",";
	obstacle_file << "\"size-info\",";
	obstacle_file << "\"\",";
	obstacle_file << "\"\"" << endl;
}

void SimulationKernel::write_obstacle(ofstream obstacle_file, boost::shared_ptr<Obstacle> obs) {
	//TODO(mmarcus) implement
}

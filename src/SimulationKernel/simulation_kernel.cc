#include "simulation_kernel.h"
#include "../Utilities/unsupported_operation_exception.h"

#include "../Model/world_information.h"
#include "../Model/sphere.h"
#include "../Model/box.h"
#include "../Model/robot_data.h"
#include "../Model/robot.h"

#include "../SimulationControl/history.h"

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
	//TODO create parser object for loading project files
	//load_projectfiles(project_filename);
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

void SimulationKernel::save_main_project_file(const string& project_filename) {

	//TODO(mmarcus) catch exceptions

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

	ofstream robot_file;

	//TODO(mmarcus) this will not work - path needs to be specified
	robot_file.open(robot_filename_.c_str());

	//write robot header
	robot_file << "\"ID\"";
	robot_file << "\"x-position\",\"y-position\",\"z-position\"";
	robot_file << "\"type\"";
	robot_file << "\"x-velocity\",\"y-velocity\",\"z-velocity\"";
	robot_file << "\"x-acceleration\",\"y-acceleration\",\"z-acceleration\"";
	robot_file << "\"status\"";
	robot_file << "\"marker-info\"";
	robot_file << "\"algorithm\"";
	robot_file << "\"color\"";
	robot_file << "\"x-axis-1\",\"x-axis-2\",\"x-axis-3\"";
	robot_file << "\"y-axis-1\",\"y-axis-2\",\"y-axis-3\"";
	robot_file << "\"z-axis-1\",\"z-axis-2\",\"z-axis-3\"" << endl;

	vector<boost::shared_ptr<RobotData> > robots_data = history_->get_newest().robot_data();

	for (vector<boost::shared_ptr<RobotData> >::iterator it = robots_data.begin();
			it!=robots_data.end(); ++it) {
		robot_file << write_robot(*it);
	}

	robot_file.close();

}

string SimulationKernel::write_robot(boost::shared_ptr<RobotData> robot_data) {

	stringstream output;

	output << robot_data->id() << ",";
	output << robot_data->position()(0) << ",";
	output << robot_data->position()(1) << ",";
	output << robot_data->position()(2) << ",";
	output << robot_data->type() << ",";	//TODO(mmarcus) Type correct?
	output << robot_data->velocity()(0) << ",";
	output << robot_data->velocity()(1) << ",";
	output << robot_data->velocity()(2) << ",";

	//TODO(mmarcus) Why does this not work?
	//boost::shared_ptr<Vector3d> vec;
	//vec = robot_data->acceleration();

	/*	output << robot_data->acceleration()(0) << ",";
	output << robot_data->acceleration()(1) << ",";
	output << robot_data->acceleration()(2) << ",";*/
	output << robot_data->status() << ",";
	//TODO(mmarcus) include marker-information
	//TODO(mmarcus) include algorithm-information
	//TODO(mmarcus) include color-information
	//TODO(mmarcus) this doesn't work and I don't know why...
	//output << robot_data->coordinate_system_axis().get<0>()(0) << ",";

	return output.str();
}

void SimulationKernel::save_obstacle_file() {

	ofstream obstacle_file;

	//TODO(mmarcus) this will not work - path needs to be specified
	obstacle_file.open(obstacle_filename_.c_str());

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
	vector<boost::shared_ptr<Obstacle> > obstacles = history_->get_newest().obstacles();

	//iterate over all obstacles
	for (vector<boost::shared_ptr<Obstacle> >::iterator it = obstacles.begin(); it!=obstacles.end(); ++it) {
		obstacle_file << write_obstacle(*it);
	}

	//fetching marker-informations from actual world-information through history-reference
	vector<boost::shared_ptr<WorldObject> > markers = history_->get_newest().markers();

	//iterate over all markers
	for (vector<boost::shared_ptr<WorldObject> >::iterator it = markers.begin(); it!=markers.end(); ++it) {
		obstacle_file << write_marker(*it);
	}

	obstacle_file.close();
}

string SimulationKernel::write_obstacle(boost::shared_ptr<Obstacle> current_obstacle) {

	stringstream output;

	//check by dynamic typecast if the obstacle is a sphere or a box.
	//See event_handler.cc for details to this technique
	if(boost::shared_ptr<Box> box_obstacle = boost::dynamic_pointer_cast<Box>(current_obstacle)) {
		output << "\"box\",";
		output << box_obstacle->position()(0) << ",";
		output << box_obstacle->position()(1) << ",";
		output << box_obstacle->position()(2) << ",";
		//TODO(mmarcus) include marker-information
		output << box_obstacle->width() << ",";
		output << box_obstacle->height() << ",";
		output << box_obstacle->depth() << endl;
	} else if(boost::shared_ptr<Sphere> sphere_obstacle = boost::dynamic_pointer_cast<Sphere>(current_obstacle)) {
		output << "\"sphere\",";
		output << sphere_obstacle->position()(0) << ",";
		output << sphere_obstacle->position()(1) << ",";
		output << sphere_obstacle->position()(2) << ",";
		//TODO(mmarcus) include marker-information
		output << sphere_obstacle->radius() << ",\"\",\"\"" << endl;
	} else {
		throw std::invalid_argument("Illegal type of obstacle.");
	}

	return output.str();
}

string SimulationKernel::write_marker(boost::shared_ptr<WorldObject> marker) {

	stringstream output;

	output << "\"marker\",";
	output << marker->position()(0) << ",";
	output << marker->position()(1) << ",";
	output << marker->position()(2) << ",";
	//TODO(mmarcus) include marker-information
	output << "\"\",\"\",\"\"" << endl;

	return output.str();
}

void SimulationKernel::save_projectfiles(const string& project_filename) {
	save_main_project_file(project_filename);
	save_robot_file();
	save_obstacle_file();
}

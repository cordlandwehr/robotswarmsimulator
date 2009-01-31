#include "simulation_kernel.h"

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
	//1.5 Create robot-control (for making viewobjects)
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
	//get next event from ASG
	//call handle_event method
}

void SimulationKernel::multistep(int steps) {
	for (int i = 0; i < steps; i++) {
		step();
	}
}

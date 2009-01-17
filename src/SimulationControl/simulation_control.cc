#include "simulation_control.h"
#include "../Model/world_information.h"
#include "../SimulationControl/history.h"


SimulationControl::SimulationControl()  {

}

SimulationControl::~SimulationControl() {

}

boost::shared_ptr<History> SimulationControl::start_new_simulation(const std::string& filename) {
	if(simulation_kernel_) {
		//terminate old simulation (thread)
		//TODO:
		//simulation_kernel_->terminate();
	}
	//create new and initialise new kernel
	simulation_kernel_.reset(new SimulationKernel());
	simulation_kernel_->init(filename);

	//set up a new history
	//TODO:
	//std::size_t history_length = simulation_kernel->history_length_(); //project parameter?
	std::size_t history_length = 123;
	history_.reset(new History(history_length));

	//give simulation_kernel the history object. May also be done through init?
	//TODO:
	//simulation_kernel_->set_history(history_);

	//start new thread
	boost::thread simulation_thread(boost::bind(&SimulationControl::run_simulation, this));
	simulation_thread_.swap(simulation_thread);

	return history_;
}

void SimulationControl::run_simulation() {
	//TODO:
	//simulation_kernel_.run();
}

#include "simulation_control.h"
#include "../Model/world_information.h"
#include "../SimulationControl/history.h"


SimulationControl::SimulationControl() : empty_count_(0), fill_count_(kBufferLength) {

}

SimulationControl::~SimulationControl() {

}

void SimulationControl::init(const std::string& filename) {
	//TODO:
	//simulation_kernel_.init(filename);
	//visualisation_kernel_.init();
}

void SimulationControl::run() {
	boost::thread simulation_thread(boost::bind(&SimulationControl::run_simulation, this));
	simulation_thread_.swap(simulation_thread);
	boost::thread visualisation_thread(boost::bind(&SimulationControl::run_visualisation, this));
	visualisation_thread_.swap(visualisation_thread);
}

const WorldInformation& SimulationControl::get_current_world_information() {
	//TODO(craupach): Better move all these Semaphore operations into the history class:
	// The event handler should not need to know about the SimulationControl class but it
	// needs to access the history in a protected way.
	fill_count_.wait();
	fill_count_.post();
	//TODO:
	return simulation_kernel_.history()->get_oldest();
}

void SimulationControl::proceed_to_next_world_information() {
	//TODO(craupach): Better move all these Semaphore operations into the history class
	fill_count_.wait();
	//TODO:
	//simulation_kernel_.history()->pop_front();
	empty_count_.post();
}

void SimulationControl::write_world_information(boost::shared_ptr<WorldInformation> info) {
	empty_count_.wait();
	simulation_kernel_.history()->push_back(info);
	fill_count_.post();
}

void SimulationControl::run_simulation() {
	//TODO:
	//simulation_kernel_.run();
}

void SimulationControl::run_visualisation() {
	//TODO:
	//visualisation_kernel_.run();
}

unsigned SimulationControl::get_time_of_next_event() const {
	//TODO:
	return 0;
}


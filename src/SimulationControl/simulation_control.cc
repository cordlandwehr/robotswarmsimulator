#include "simulation_control.h"
#include "../Model/world_information.h"
#include "../SimulationControl/history.h"
#include "../SimulationKernel/simulation_kernel.h"
#include "../Model/world_information.h"
#include "visualizer.h"
#include "history.h"

#include <iostream>


namespace {
	bool is_thread_started(const boost::thread& thread) {
		return thread.get_id() != boost::thread::id();
	}
}

SimulationControl::SimulationControl() : processing_time_factor_(10), current_processing_time_(0)  {

}

SimulationControl::~SimulationControl() {

}

void SimulationControl::create_new_simulation(const std::string& configuration_filename, std::size_t history_length) {
	// terminate the old simulation including the old simulation thread.
	terminate_simulation();

	//set up a new history
	history_.reset(new History(history_length));

	// create and initialize new kernel. History should be passed here because the init method
	// of the SimulationKernel will need it to construct EventHandler, ASG, StatisticKernel.
	boost::shared_ptr<SimulationKernel> simulation_kernel(new SimulationKernel());
	simulation_kernel->init(configuration_filename, history_);

	simulation_kernel_functor_.reset(new SimulationKernelFunctor(simulation_kernel));

	current_processing_time_ = 0;
}

void SimulationControl::start_simulation() {
	if(!is_thread_started(simulation_thread_)) {
		boost::thread simulation_thread(boost::bind(&SimulationKernelFunctor::loop, simulation_kernel_functor_));
		simulation_thread_.swap(simulation_thread);

		//fetch first two WorldInformations
		current_world_information_ = history_->get_oldest_unused(true);
		next_world_information_ = history_->get_oldest_unused(true);

		//some initialization
		current_processing_time_ = current_world_information_->time() * processing_time_factor_;
		last_process_simulation_time_ = boost::posix_time::microsec_clock::local_time();
	}
	else {
		simulation_kernel_functor_->unpause();
	}
}

void SimulationControl::pause_simulation() {
	if(is_thread_started(simulation_thread_)) {
		simulation_kernel_functor_->pause();
	}
}

void SimulationControl::terminate_simulation() {
	if(is_thread_started(simulation_thread_)) {
		simulation_kernel_functor_->terminate();
		//note: terminate above will have no effect if the thread is blocked, so unblock it
		history_->get_oldest_unused();

		simulation_thread_ = boost::thread();
		simulation_kernel_functor_ = boost::shared_ptr<SimulationKernelFunctor>();
	}
}

void SimulationControl::process_simulation() {
	//std::cout << "processing" << std::endl;
	double new_processing_time = compute_new_processing_time();

	// Try and get the two world informations matching our processing time.
	// This may skip world informations if we are processing too fast.
	// (may happen if the simulation time distance between two world informations
	// has a high variance. We would need to dynamically adjust the processing time factor
	// if we would want to avoid this).
	// If the Simulation has not produced the needed world informations yet, we pause
	// the processing time.
	double current_simulation_time = new_processing_time/processing_time_factor_;
	while(current_simulation_time >= next_world_information_->time()) {
		boost::shared_ptr<WorldInformation> new_world_information = history_->get_oldest_unused();
		if(!new_world_information) {
			//try_wait failed
			//proceed processing_time to next_world_information time instead of setting it to new_processing_time
			//-> pauses processing_time at next_world_information_->time()
			current_processing_time_ = next_world_information_->time() * processing_time_factor_;
			std::cout << "try wait failed" << std::endl;
			break;
		}
		else {
			current_world_information_ = next_world_information_;
			next_world_information_ = new_world_information;
			current_processing_time_ = new_processing_time;
			std::cout << "fetched new world information, processing time:" << current_processing_time_ << std::endl;
		}
	}

	// draw the simulation state for the current processing time if there is a visualizer.
	if(visualizer_) {
		double extrapolation_time = current_processing_time_/processing_time_factor_ - current_world_information_->time();
		//std::cout << "start drawing time: " << extrapolation_time << std::endl;
		//std::cout << "for world information at time: " << current_world_information_->time() << std::endl;
		visualizer_->draw(extrapolation_time, current_world_information_);
		//std::cout << "end drawing" << std::endl;
	}

}

double SimulationControl::compute_new_processing_time() {
	boost::posix_time::ptime current_time = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration time_elapsed = current_time - last_process_simulation_time_;
	last_process_simulation_time_ = current_time;
	return current_processing_time_ + time_elapsed.total_milliseconds();
}

SimulationControl::SimulationKernelFunctor::SimulationKernelFunctor(boost::shared_ptr<SimulationKernel> simulation_kernel)
                                                : terminated_(false), paused_(false), unpaused_(1), simulation_kernel_(simulation_kernel) {

}

void SimulationControl::SimulationKernelFunctor::unpause() {
	if(paused_) {
		paused_ = false;
		//set unpaused to 1
		unpaused_.post();
	}
}

void SimulationControl::SimulationKernelFunctor::pause() {
	if(!paused_) {
		paused_ = true;
		//set unpaused to 0
		unpaused_.wait();
	}
}

void SimulationControl::SimulationKernelFunctor::terminate() {
	terminated_ = true;
}

void SimulationControl::SimulationKernelFunctor::loop() {
	while(!terminated_) {
		//wait if unpaused == 0
		unpaused_.wait();
		unpaused_.post();

		simulation_kernel_->step();
	}
	simulation_kernel_->quit();
}

void SimulationControl::set_visualizer(boost::shared_ptr<Visualizer> visualizer) {
	visualizer_ = visualizer;
}

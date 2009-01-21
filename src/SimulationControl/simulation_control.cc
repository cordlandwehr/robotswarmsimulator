#include "simulation_control.h"
#include "../Model/world_information.h"
#include "../SimulationControl/history.h"
#include "../SimulationKernel/simulation_kernel.h"
#include "../Model/world_information.h"
#include "visualizer.h"
#include "history.h"


namespace {
	bool is_thread_started(const boost::thread& thread) {
		return thread.get_id() != boost::thread::id();
	}

	long current_time_in_millisec() {
		//TODO:
		return 0;
	}
}

SimulationControl::SimulationControl() : processing_time_factor_(1000), current_processing_time_(0)  {

}

SimulationControl::~SimulationControl() {

}

void SimulationControl::create_new_simulation(const std::string& configuration_filename) {
	//TODO (dwonisch): does this method terminate the old simulation thread?
	//                 I assume it does.
	terminate_simulation();

	//create new and initialise new kernel
	boost::shared_ptr<SimulationKernel> simulation_kernel(new SimulationKernel());
	simulation_kernel->init(configuration_filename);

	//set up a new history
	//TODO:
	//std::size_t history_length = simulation_kernel->history_length_(); //project parameter?
	std::size_t history_length = 123;
	history_.reset(new History(history_length));

	//give simulation_kernel the history object. May also be done through init?
	//TODO:
	//simulation_kernel_->set_history(history_);

	simulation_kernel_functor_.reset(new SimulationKernelFunctor(simulation_kernel));

	current_processing_time_ = 0;
}

void SimulationControl::start_simulation() {
	if(!is_thread_started(simulation_thread_)) {
		boost::thread simulation_thread(boost::bind(&SimulationKernelFunctor::operator(), simulation_kernel_functor_));
		simulation_thread_.swap(simulation_thread);
		//fetch first two WorldInformations
		current_world_information_ = history_->get_oldest_unused(true);
		next_world_information_ = history_->get_oldest_unused(true);
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
	//compute new processing time
	long current_time = current_time_in_millisec();
	double new_processing_time = current_processing_time_ + (current_time - last_process_simulation_time_);
	last_process_simulation_time_ = current_time;

	double current_simulation_time = new_processing_time/processing_time_factor_;
	while(current_simulation_time >= next_world_information_->time()) {
		boost::shared_ptr<WorldInformation> new_world_information = history_->get_oldest_unused();
		if(!new_world_information) {
			//try_wait failed
			//proceed processing_time to next_world_information time instead of setting it to new_processing_time
			//-> pauses processing_time at next_world_information_->time()
			current_processing_time_ = next_world_information_->time() * processing_time_factor_;
			break;
		}
		else {
			current_world_information_ = next_world_information_;
			next_world_information_ = new_world_information;
			current_processing_time_ = new_processing_time;
		}
	}
	if(visualizer_) {
		double extrapolation_time = current_processing_time_/processing_time_factor_ - current_world_information_->time();
		visualizer_->draw(extrapolation_time, current_world_information_);
	}
}

void SimulationControl::set_visualizer(boost::shared_ptr<Visualizer> visualizer) {
	visualizer_ = visualizer;
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

void SimulationControl::SimulationKernelFunctor::operator()() {
	while(!terminated_) {
		//wait if unpaused == 0
		unpaused_.wait();
		unpaused_.post();

		simulation_kernel_->step();
	}
}

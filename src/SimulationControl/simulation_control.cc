/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>

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

#include "simulation_control.h"
#include "../Model/world_information.h"
#include "../SimulationControl/history.h"
#include "../SimulationKernel/simulation_kernel.h"
#include "../Model/world_information.h"
#include "visualizer.h"
#include "history.h"
#include "time_point.h"
#include <Utilities/console_output.h>
#include <ctime>

#include <iostream>


namespace {
	bool is_thread_started(const boost::thread& thread) {
		return thread.get_id() != boost::thread::id();
	}
}

SimulationControl::SimulationControl() : processing_time_factor_(1000), current_processing_time_(0),
                                         processing_time_delta_(1), limit_processing_time_(0),
                                         single_step_mode_(false), processing_time_paused_(false) {

}

SimulationControl::~SimulationControl() {

}

void SimulationControl::create_new_simulation(const std::string& configuration_filename,
		                                      std::size_t history_length,
		                                      std::string output_dir="",
		                                      bool limited_steps=false,
		                                      int number_of_steps=0,
		                                      bool run_until_no_multiplicity=false) {
	// terminate the old simulation including the old simulation thread.
	terminate_simulation();

	//set up a new history
	history_.reset(new History(history_length));

	// create and initialize new kernel. History should be passed here because the init method
	// of the SimulationKernel will need it to construct EventHandler, ASG, StatisticKernel.
	boost::shared_ptr<SimulationKernel> simulation_kernel(new SimulationKernel());
	simulation_kernel->init(configuration_filename, history_, output_dir);

	camera_position_ = simulation_kernel->camera_position();
	camera_direction_ = simulation_kernel->camera_direction();
	camera_type_ = simulation_kernel->camera_type();

	if(!limited_steps) {
		simulation_kernel_functor_.reset(new SimulationKernelFunctor(simulation_kernel, run_until_no_multiplicity));
	} else {
		simulation_kernel_functor_.reset(new SimulationKernelFunctor(simulation_kernel, run_until_no_multiplicity, number_of_steps));
	}
	current_processing_time_ = 0;
}

void SimulationControl::start_simulation() {
	if(!is_thread_started(simulation_thread_)) {
		boost::thread simulation_thread(boost::bind(&SimulationKernelFunctor::loop, simulation_kernel_functor_));
		simulation_thread_.swap(simulation_thread);

		//fetch the first two WorldInformations
		current_time_point_ = history_->get_oldest_unused(true);
		next_time_point_ = history_->get_oldest_unused(true);

		//some initialization
		current_processing_time_ = current_time_point_->world_information().time() * processing_time_factor_; ;
		last_process_simulation_time_ = boost::posix_time::microsec_clock::local_time();
	} else {
		simulation_kernel_functor_->unpause();
	}
}

void SimulationControl::pause_simulation() {
	if(is_thread_started(simulation_thread_)) {
		simulation_kernel_functor_->pause();
	}
}

void SimulationControl::enter_single_step_mode() {
	single_step_mode_ = true;
	limit_processing_time_ = current_processing_time_;
	pause_processing_time(true);
}

void SimulationControl::do_single_step() {
	limit_processing_time_ += processing_time_factor_;
	pause_processing_time(false);
}

void SimulationControl::exit_single_step_mode() {
	single_step_mode_ = false;

	// unpause is nop if not paused
	pause_processing_time(false);
}

void SimulationControl::terminate_simulation() {
	if(is_thread_started(simulation_thread_)) {
		simulation_kernel_functor_->terminate();
		// note: terminate above will have no effect if the thread is blocked, so unblock it
		history_->get_oldest_unused();

		// simulation thread has up to three seconds to shut down; if it does not, issue a warning
		if (!simulation_thread_.timed_join(boost::posix_time::seconds(3)))
			ConsoleOutput::log(ConsoleOutput::Control, ConsoleOutput::warning) << "Simulation thread seems to have deadlocked, terminating anyway" ;
		simulation_thread_ = boost::thread();
		simulation_kernel_functor_ = boost::shared_ptr<SimulationKernelFunctor>();
	}
}

bool SimulationControl::is_simulation_finished() {
	return simulation_kernel_functor_->is_terminated();
}

void SimulationControl::process_simulation() {
	double new_processing_time = compute_new_processing_time();
	// Try and get the two world informations matching our processing time.
	// This may skip world informations if we are processing too fast.
	// (may happen if the simulation time distance between two world informations
	// has a high variance. We would need to dynamically adjust the processing time factor
	// if we would want to avoid this).
	// If the Simulation has not produced the needed world informations yet, we pause
	// the processing time.
	double current_simulation_time = new_processing_time/processing_time_factor_;
	if(single_step_mode_ && current_processing_time_ > limit_processing_time_) {
		pause_processing_time(true);
	}

	while(current_simulation_time >= next_time_point_->world_information().time()) {
		boost::shared_ptr<TimePoint> new_time_point = history_->get_oldest_unused();
		if(!new_time_point) {
			// try_wait failed
			// proceed processing_time to next_world_information time instead of setting it to new_processing_time
			// -> pauses processing_time at next_world_information_->time()
			current_processing_time_ = next_time_point_->world_information().time() * processing_time_factor_;
			draw_current_simulation();
			return;
		}
		else {
			current_time_point_ = next_time_point_;
			next_time_point_ = new_time_point;
		}
	}
	current_processing_time_ = new_processing_time;
	draw_current_simulation();
}

void SimulationControl::draw_current_simulation()
{
	// draw the simulation state for the current processing time if there is a visualizer.
	if(visualizer_) {
		double extrapolation_time = current_processing_time_/processing_time_factor_ - current_time_point_->world_information().time();
		visualizer_->draw(extrapolation_time, current_time_point_);
	}
}

double SimulationControl::compute_new_processing_time() {
	boost::posix_time::ptime current_time = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration time_elapsed = current_time - last_process_simulation_time_;
	last_process_simulation_time_ = current_time;
	return current_processing_time_ + processing_time_delta_*time_elapsed.total_milliseconds();
}

SimulationControl::SimulationKernelFunctor::SimulationKernelFunctor(boost::shared_ptr<SimulationKernel> simulation_kernel,
		                                                            bool run_until_no_multiplicity) : terminated_(false),
		                                                                                              paused_(false),
		                                                                                              limited_steps_(false),
		                                                                                              number_of_steps_(0),
		                                                                                              totalcalctime_(0),
		                                                                                              run_until_no_multiplicity_(run_until_no_multiplicity),
		                                                                                              unpaused_(1),
		                                                                                              simulation_kernel_(simulation_kernel) {}

SimulationControl::SimulationKernelFunctor::SimulationKernelFunctor(boost::shared_ptr<SimulationKernel> simulation_kernel,
																	bool run_until_no_multiplicity,
                                                                    int number_of_steps) : terminated_(false),
																						   paused_(false),
																						   limited_steps_(true),
																						   number_of_steps_(number_of_steps),
																						   totalcalctime_(0),
																						   run_until_no_multiplicity_(run_until_no_multiplicity),
																						   unpaused_(1),
																						   simulation_kernel_(simulation_kernel) {}

void SimulationControl::increase_processing_time_exp(){
	if (processing_time_delta_<1000)	{
		processing_time_delta_=2*processing_time_delta_;
	}
}

void SimulationControl::decrease_processing_time_exp(){
	if (processing_time_delta_>0.002){
		processing_time_delta_=processing_time_delta_/2;
	}

}


void SimulationControl::pause_processing_time(bool pause){
	if (!pause && processing_time_paused_){
		// resuming
		processing_time_delta_ = old_processing_time_delta_;
		processing_time_paused_ = false;
	}
	else if(pause && !processing_time_paused_) {
		// pausing
		old_processing_time_delta_= processing_time_delta_;
		processing_time_delta_=0;
		processing_time_paused_ = true;
	}
}

void SimulationControl::decrease_processing_time_linearly(){
	if (processing_time_delta_>1.1){
		processing_time_delta_--;
	}
}
void SimulationControl::increase_processing_time_linearly(){
	if (processing_time_delta_<1000){
		processing_time_delta_++;
	}
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
	int steps = 0;
	while(!terminated_) {
		try{
			//wait if unpaused == 0
			unpaused_.wait();
			unpaused_.post();

			long curStepClock = clock();

			simulation_kernel_->step();

			curStepClock = (clock() - curStepClock);
			totalcalctime_ += curStepClock;
			double avgtime = totalcalctime_ * 1000.0 / CLOCKS_PER_SEC / (steps+1);
			double curtime = curStepClock * 1000.0 / CLOCKS_PER_SEC;

			if(limited_steps_) {
				if (steps % 128 == 0)				
					ConsoleOutput::log(ConsoleOutput::Control, ConsoleOutput::info) << "completed step " << steps << "/" << number_of_steps_ << " with " << curtime << " ms (avg is " << avgtime << " ms)";

				steps++;
				if( steps > number_of_steps_) {
					terminate();
				}
			}

			// check for a termination condition
			if(simulation_kernel_->terminate_condition(run_until_no_multiplicity_)) {
				terminate();
			}
		}
		catch(std::exception& e) {
			ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << e.what();
			terminate();
		}
		catch(...) {
			ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "Uncaught unknown exception.\n";
			terminate();
		}
	}
	simulation_kernel_->quit();
}

void SimulationControl::set_visualizer(boost::shared_ptr<Visualizer> visualizer) {
	visualizer_ = visualizer;
}


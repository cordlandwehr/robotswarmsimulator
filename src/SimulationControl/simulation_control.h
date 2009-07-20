/**
 * \class	SimulationControl
 * \author	Daniel Wonisch
 * \brief	Controls the simulation.
 */


#ifndef SIMULATION_CONTROL_H_
#define SIMULATION_CONTROL_H_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <cstddef>
#include <string>

class SimulationKernel;
class Visualizer;
class WorldInformation;
class TimePoint;
class History;

/**
 * The class SimulationControl provides code to set up a new simulation (and cleanup
 * any old one) as well as start/pause/terminate the Simulation-Thread. Furthermore it
 * provides a method called process simulation that is responsible for consuming the
 * history's elements according to the Processing-Time.
 */
class SimulationControl {
public:
	SimulationControl();
	~SimulationControl();

	/**
	 * cleans up the old simulation and creates a new one without starting it.
	 */
	void create_new_simulation(const std::string& configuration_filename,
	                           std::size_t history_length,
	                           std::string ouput_dir,
	                           bool create_statistics,
	                           bool limited_steps,
	                           int number_of_steps);

	/**
	 * change processing_time_delta (see below)
	 */
	void set_processing_time_delta(double processing_time_delta) {processing_time_delta_ = processing_time_delta;}
	void increase_processing_time_linearly();
	void decrease_processing_time_linearly();
	void increase_processing_time_exp();
	void decrease_processing_time_exp();

	/**
	 * pauses or unpauses the processing time depending on the passed value for pause.
	 */
	void pause_processing_time(bool pause);
	bool is_processing_time_paused() {return processing_time_paused_;}

	/**
	 * starts the simulation
	 */
	void start_simulation();

	/**
	 * pauses the simulation
	 */
	void pause_simulation();

	/**
	 * members for control of the single step mode
	 */
	void enter_single_step_mode();
	void do_single_step();
	void exit_single_step_mode();
	bool is_single_step_mode() { return single_step_mode_; }

	/**
	 * terminates simulation and cleans up the simulation thread
	 */
	void terminate_simulation();

	/**
	 * returns true if the functor of the simulation is terminated
	 */
	bool is_simulation_finished();

	/**
	 * process the simulation: advances the processing time and calls the visualizer if there is one.
	 */
	void process_simulation();

	/**
	 * supplies the control with a new visualizer.
	 */
	void set_visualizer(boost::shared_ptr<Visualizer> visualizer);

	/**
	 * Used to tell the Functor to tell the Simkernel to save the current configuration
	 */
	void dump_simulation();

	std::string camera_position(){return camera_position_; }
	std::string camera_direction(){ return camera_direction_;}
	std::string camera_type(){ return camera_type_;}

private:
	/**
	 * Thread-Wrapper for the simulation kernel. Allows the simulation kernel thread to be paused and
	 * unpaused.
	 */
	class SimulationKernelFunctor {
	public:
		/**
		 * constructs a new functor with the given simulation kernel thread.
		 */
		SimulationKernelFunctor(boost::shared_ptr<SimulationKernel> simulation_kernel);

		/**
		 * constructs a new functor with the given simulation kernel thread and
		 * limits the number of steps to be simulated
		 */
		SimulationKernelFunctor(boost::shared_ptr<SimulationKernel> simulation_kernel,
		                        int number_of_steps);

		/**
		 * unpauses the simulation thread using a semaphor
		 */
		void unpause();



		/**
		 * pauses the simulation thread using a semaphor
		 */
		void pause();

		/**
		 * terminates the simulation thread by exiting the endless loop
		 */
		void terminate();

		/**
		 * a endless loop performing simulation steps until terminated.
		 */
		void loop();

		/**
		 * returns true iff the functor is terminated
		 */
		bool is_terminated() {return terminated_;};

		/**
		 * Used to tell the Simkernel to save the current configuration
		 */
		void dump_simulation();


	private:
		/**
		 * true iff the endless loop belonging to this functor should exit / is already exited.
		 */
		bool terminated_;

		/**
		 * true iff the thread should pause / is already pausing.
		 */
		bool paused_;

		/**
		 * true iff the thread should automatically terminate after a fixed number of steps
		 */
		bool limited_steps_;
		int number_of_steps_;


		boost::interprocess::interprocess_semaphore unpaused_;
		boost::shared_ptr<SimulationKernel> simulation_kernel_;
	};

	double compute_new_processing_time();
	void draw_current_simulation();


private:
	//TODO (dwonsich): processing_time_factor_ == \Delta in paper; Maybe should be moved to somewhere else
	double processing_time_factor_;
	double current_processing_time_;
	boost::posix_time::ptime last_process_simulation_time_;

	boost::shared_ptr<TimePoint> current_time_point_;
	boost::shared_ptr<TimePoint> next_time_point_;
	boost::shared_ptr<History> history_;
	boost::shared_ptr<SimulationKernelFunctor> simulation_kernel_functor_;
	boost::thread simulation_thread_;
	boost::shared_ptr<Visualizer> visualizer_;

	/**
	 * alters how fast processing time advances
	 */
	double processing_time_delta_;

	/**
	 * temporary processing time delta value for pausing
	 */
	double old_processing_time_delta_;

	std::string camera_position_;
	std::string camera_direction_;
	std::string camera_type_;

	/**
	 * if we reach this time, the simulation is paused
	 */
	double limit_processing_time_;

	/**
	 * true iff we are in single step mode
	 */
	bool single_step_mode_;

	/**
	 * true iff processing time is paused
	 */
	bool processing_time_paused_;
};

#endif


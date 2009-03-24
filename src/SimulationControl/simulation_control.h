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
	void create_new_simulation(const std::string& configuration_filename, std::size_t history_length, std::string ouput_dir, bool create_statistics);

	/**
	 * change processing_time_delta (see below)
	 */
	void increase_processing_time_linearly();
	void decrease_processing_time_linearly();
	void increase_processing_time_exp();
	void decrease_processing_time_exp();
	void pause_processing_time();

	/**
	 * starts the simulation
	 */
	void start_simulation();

	/**
	 * pauses the simulation
	 */
	void pause_simulation();

	/**
	 * terminates simulation and cleans up the simulation thread
	 */
	void terminate_simulation();

	/**
	 * process the simulation: advances the processing time and calls the visualizer if there is one.
	 */
	void process_simulation();

	/**
	 * supplies the control with a new visualizer.
	 */
	void set_visualizer(boost::shared_ptr<Visualizer> visualizer);

	std::string camera_position(){return camera_position_; }
	std::string camera_direction(){ return camera_direction_;}

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

	private:
		/**
		 * true iff the endless loop belonging to this functor should exit / is already exited.
		 */
		bool terminated_;

		/**
		 * true iff the thread should pause / is already pausing.
		 */
		bool paused_;

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

	boost::shared_ptr<WorldInformation> current_world_information_;
	boost::shared_ptr<WorldInformation> next_world_information_;
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
};

#endif


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
#include <cstddef>


class SimulationKernel;
class Visualizer;
class WorldInformation;
class History;
class GUI;

class SimulationControl {
public:
	SimulationControl();
	~SimulationControl();

	void create_new_simulation(const std::string& configuration_filename);
	void start_simulation();
	void pause_simulation();
	void terminate_simulation();
	void process_simulation();
	void set_visualizer(boost::shared_ptr<Visualizer> visualizer);
	void set_gui(boost::shared_ptr<GUI> gui);
	static boost::shared_ptr<SimulationControl> getInstance();
	static void setInstance(boost::shared_ptr<SimulationControl>);


private:

	class SimulationKernelFunctor {
	public:
		SimulationKernelFunctor(boost::shared_ptr<SimulationKernel> simulation_kernel);
		void unpause();
		void pause();
		void terminate();

		void operator()();

	private:
		bool terminated_;
		bool paused_;
		//TODO (dwonisch): Semaphore should do the job, but there might be better solutions.
		boost::interprocess::interprocess_semaphore unpaused_;
		boost::shared_ptr<SimulationKernel> simulation_kernel_;
	};





private:
	//TODO (dwonsich): processing_time_factor_ == \Delta in paper; Maybe should be moved to somewhere else
	double processing_time_factor_;
	double current_processing_time_;
	static boost::shared_ptr<SimulationControl> simcontrol_instance_;
	//TODO:
	long last_process_simulation_time_;

	boost::shared_ptr<WorldInformation> current_world_information_;
	boost::shared_ptr<WorldInformation> next_world_information_;
	boost::shared_ptr<History> history_;
	boost::shared_ptr<SimulationKernelFunctor> simulation_kernel_functor_;
	boost::thread simulation_thread_;
	boost::shared_ptr<Visualizer> visualizer_;
	boost::shared_ptr<GUI> gui_;

};

#endif


/**
 * \class	SimulationControl
 * \author	Daniel Wonisch
 * \brief	Controls the simulation.
 */


#ifndef SIMULATION_CONTROL_H_
#define SIMULATION_CONTROL_H_

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <cstddef>

#include "../SimulationKernel/simulation_kernel.h"
//TODO:
//#include "../Visualisation/visualisation_kernel.h"

class WorldInformation;


//TODO: use puffer length from history?
std::size_t kBufferLength = 10;

class SimulationControl {
public:
	SimulationControl();
	~SimulationControl();

	/**
	* Initializes the SimulationKernel and VisualisationKernel.
	* @param filename Projectfile for the simulation
	*/
	void init(const std::string& filename);

	/**
	* Starts both SimulationKernel and VisualisationKernel thread.
	*/
	void run();

	/**
	* Returns the currentWorldInformation that should be visualised by the VisualisationKernel.
	* @return const ref to the current WorldInformation
	*/
	const WorldInformation& get_current_world_information();

	/**
	* Tells the Simulation that the VisualizationKernel finished visualising the current WorldInformation. May pause the calling thread, if there are no new
	* WorldInformations available.
	*/
	void proceed_to_next_world_information();

	/**
	* Writes the given WorldInformation into the buffer/history. May pause the calling thread (SimulationKernel), if the VisualisationKernel did not
	* consume enough WorldInformations yet.
	*/
	void write_world_information(boost::shared_ptr<WorldInformation> info);

	/**
	* blabla, prolly changes anyway
	*/
	unsigned get_time_of_next_event() const;

	//TODO: other methods...
private:
	/**
	 * This method is called when the simulation thread is started
	 */
	void run_simulation();
	/**
	* This method is called when the visualisation thread is started
	*/
	void run_visualisation();

private:
	SimulationKernel simulation_kernel_;
	//TODO:
	//VisualizationKernel visualisation_kernel_;
	boost::thread simulation_thread_;
	boost::thread visualisation_thread_;
	boost::interprocess::interprocess_semaphore empty_count_;
	boost::interprocess::interprocess_semaphore fill_count_;
};

#endif


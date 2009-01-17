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
#include <cstddef>

#include "../SimulationKernel/simulation_kernel.h"


//TODO: use puffer length from history?
std::size_t kBufferLength = 10;

class SimulationControl {
public:
	SimulationControl();
	~SimulationControl();

	boost::shared_ptr<History> start_new_simulation(const std::string& filename);

private:
	/**
	 * This method is called when the simulation thread is started
	 */
	void run_simulation();

private:
	boost::shared_ptr<History> history_;
	boost::shared_ptr<SimulationKernel> simulation_kernel_;
	boost::thread simulation_thread_;
};

#endif


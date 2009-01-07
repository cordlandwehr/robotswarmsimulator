/*
 * simulation_kernel.cc
 *
 *  Created on: Jan 7, 2009
 *      Author: martinah
 */

#include "simulation_kernel.h"

SimulationKernel::SimulationKernel() {
	// TODO Auto-generated constructor stub

}

SimulationKernel::~SimulationKernel() {
	// TODO Auto-generated destructor stub
}

const vector<boost::shared_ptr<Robot> >& SimulationKernel::robots() const {
	return robots_;
}

const boost::shared_ptr<History>& SimulationKernel::history() const {
	return history_;
}

/*
 * simulation_listener.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef SIMULATION_LISTENER_H_
#define SIMULATION_LISTENER_H_

#include <boost/smart_ptr.hpp>

class WorldInformation;
class HandleRequestsEvent;

class SimulationListener {
public:
	virtual ~SimulationListener() {}

	/**
	 * updates the listener after the HandleRequestsEvent event has happened.
	 * world_information is guranteed to be a pointer to the most recently generated WorldInformation object.
	 */
	virtual update(boost::shared_ptr<WorldInformation> world_information,
			       boost::shared_ptr<HandleRequestsEvent> event) = 0;
};

#endif /* SIMULATION_LISTENER_H_ */

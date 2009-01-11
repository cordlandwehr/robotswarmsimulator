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
class Event;

/**
 * SimulationListener is an interface to be implemented by classes which wish to be informed of handled
 * events.
 */
class SimulationListener {
public:
	virtual ~SimulationListener() {}

	/**
	 * updates the listener after a event has happened.
	 * World_information is guaranteed to be a pointer to the most recently generated WorldInformation object.
	 * It is not guaranteed that last_event is a handle_request event and that world_information was generated
	 * for last_event.
	 *
	 * \param A constant refrence to the newest world information
	 * \param The last handled event
	 */
	virtual void update(const WorldInformation& world_information,
			            boost::shared_ptr<Event> last_event) = 0;
};

#endif /* SIMULATION_LISTENER_H_ */

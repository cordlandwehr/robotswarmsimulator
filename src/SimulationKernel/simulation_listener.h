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
class TimePoint;

/**
 * SimulationListener is an interface to be implemented by classes which wish to be informed of handled
 * events.
 */
class SimulationListener {
public:
	virtual ~SimulationListener() {}

	/**
	 * updates the listener after a event has happened.
	 * The time point is guaranteed to be a pointer to the most recently generated TimePoint object.
	 * It is not guaranteed that last_event is a handle_request event and that time_point was generated
	 * for last_event.
	 * The time point will generally not be in the history at the time of calling.
	 *
	 * \param A reference to a time point containing the newest (locked!) world information
	 * \param The last handled event
	 */
	virtual void update(TimePoint& time_point,
			            boost::shared_ptr<Event> last_event) = 0;
};

#endif /* SIMULATION_LISTENER_H_ */

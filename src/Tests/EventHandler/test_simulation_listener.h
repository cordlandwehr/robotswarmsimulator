/*
 * test_simulation_listener.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef TEST_SIMULATION_LISTENER_H_
#define TEST_SIMULATION_LISTENER_H_

#include "../../SimulationKernel/simulation_listener.h"

class Event;
class WorldInformation;

/**
 * \brief Test Class which logs the information passed by the event handler
 */
class TestSimulationListener : public SimulationListener {
public:
	TestSimulationListener() : times_update_called_(0) {}
	void update(const TimePoint& time_point,
				boost::shared_ptr<Event> last_event);

	int times_update_called() {return times_update_called_;}
	const WorldInformation& last_world_information() {return last_world_information_;}
	boost::shared_ptr<Event> last_event() {return last_event_;}

private:
	int times_update_called_;
	WorldInformation last_world_information_;
	boost::shared_ptr<Event> last_event_;
};


#endif /* TEST_SIMULATION_LISTENER_H_ */

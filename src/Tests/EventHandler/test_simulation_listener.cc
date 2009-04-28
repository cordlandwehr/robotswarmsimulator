/*
 * test_simulation_listener.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "../../Model/world_information.h"
#include "../../Events/event.h"
#include "../../SimulationControl/time_point.h"
#include "test_simulation_listener.h"

#include <iostream>

void TestSimulationListener::update(TimePoint& time_point,
                                    boost::shared_ptr<Event> last_event) {
	times_update_called_++;
	last_world_information_ = time_point.world_information();
	last_event_ = last_event;
}

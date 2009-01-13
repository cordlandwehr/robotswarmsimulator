/*
 * test_simulation_listener.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "../../Model/world_information.h"
#include "../../Events/event.h"
#include "test_simulation_listener.h"

#include <iostream>

void TestSimulationListener::update(const WorldInformation& world_information,
                                    boost::shared_ptr<Event> last_event) {
	times_update_called_++;
	last_world_information_ = world_information;
	last_event_ = last_event;
}

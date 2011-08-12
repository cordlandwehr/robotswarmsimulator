/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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
	void update(TimePoint& time_point,
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

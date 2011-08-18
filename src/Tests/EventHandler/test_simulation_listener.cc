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

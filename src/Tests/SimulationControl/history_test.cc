/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

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

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Events/look_event.h"
#include "../../Events/event.h"
#include "../../SimulationControl/history.h"
#include "../../SimulationControl/time_point.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../Fixtures/simple_world_fixture.h"

BOOST_FIXTURE_TEST_CASE(history_test, SimpleWorldFixture)
{
	boost::shared_ptr<WorldInformation> second_world_information;
	second_world_information.reset(new WorldInformation());
	second_world_information->set_time(1);
	boost::shared_ptr<TimePoint> second_time_point(new TimePoint());
	second_time_point->set_world_information(second_world_information);

	history->insert(second_time_point);

	// newest shoud be the second world information
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 1);

	// oldest should be the initial world information now be the initial one
	BOOST_CHECK_EQUAL(history->get_oldest_unused()->world_information_ptr()->time(), 0);

	// newest shoud be the second world information
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 1);

	// initial world info should be consumed now. So oldest should be the second world information.
	BOOST_CHECK_EQUAL(history->get_oldest_unused()->world_information_ptr()->time(), 1);


}

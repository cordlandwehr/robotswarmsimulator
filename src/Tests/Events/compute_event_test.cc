/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
#include "../../Events/compute_event.h"
#include "../../Model/robot.h"
#include "../Fixtures/simple_world_fixture.h"

/*
 * Helper function to test adding a robot in a different scope
 */
void add_robot(ComputeEvent& event) {
    boost::shared_ptr<Robot> robot;
    boost::shared_ptr<RobotIdentifier> id;
    id.reset(new RobotIdentifier(0));
    robot.reset(new SimpleRobot(id));
    event.add_to_robot_subset(robot);
}

BOOST_AUTO_TEST_CASE(compute_event_smoke_test)
{
    ComputeEvent event(5);

    // time correct
    BOOST_CHECK_EQUAL(event.time(), 5);

    // no robots in the subset at beginning
    BOOST_CHECK_EQUAL(event.robot_subset().size(), 0);

    // create a shared pointer on a robot
    boost::shared_ptr<Robot> robot;
    boost::shared_ptr<RobotIdentifier> id;
    id.reset(new RobotIdentifier(0));
    robot.reset(new SimpleRobot(id));
    event.add_to_robot_subset(robot);

    // there should be a robot in the subset now
    BOOST_CHECK_EQUAL(event.robot_subset().size(), 1);

	// add a robot in a different scope
    add_robot(event);

    // there shoud be two robots in the subset now
    BOOST_CHECK_EQUAL(event.robot_subset().size(), 2);
}

/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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
#include <boost/test/floating_point_comparison.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Events/look_event.h"
#include "../../Events/handle_requests_event.h"
#include "../../Events/event.h"
#include "../../Requests/request.h"
#include "../../Requests/position_request.h"
#include "../../SimulationControl/history.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../../Views/view_factory.h"
#include "../../Views/view.h"
#include "../../SimulationKernel/uniform_robot_control.h"
#include "test_simulation_listener.h"

#include "../Fixtures/simple_world_fixture.h"

#include <iostream>

/*
 *  Test: pass an empty look event for t = 5
 *  Expected Results:
 *    WorldInformation extrapolated to t = 5
 *    Last WorldInformation  at t = 5
 *    All Listeners Update Functions called once
 *    All Listeners last event is look event at t = 5
 *    All listeners last world information is the new one at t = 5
 *    Robot B is at (1.0, 0.5, 3.0)
 */
BOOST_FIXTURE_TEST_CASE(event_handler_test, SimpleWorldFixture)
{
	// Test set up: EventHandler and Listeners
	boost::shared_ptr<AbstractViewFactory> view_factory;
	view_factory.reset(new ViewFactory<View> ());

	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	EventHandler event_handler(history, robot_control);


	boost::shared_ptr<TestSimulationListener> listener_a;
	listener_a.reset(new TestSimulationListener());

	boost::shared_ptr<TestSimulationListener> listener_b;
	listener_b.reset(new TestSimulationListener());

	event_handler.register_listener(listener_a);
	event_handler.register_listener(listener_b);

	// Test Set up: Constructing Event and passing it to the handler
    boost::shared_ptr<LookEvent> look_event;
    look_event.reset(new LookEvent(5));
	boost::shared_ptr<TimePoint> time_point(new TimePoint());
	event_handler.handle_event(look_event, *time_point);
	history->insert(time_point);

	// newest world information is for t = 5
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 5);

	// all listeners update functions called once
	BOOST_CHECK_EQUAL(listener_a->times_update_called(), 1);
	BOOST_CHECK_EQUAL(listener_b->times_update_called(), 1);

	// all listeners last event is look event at t = 5
	BOOST_CHECK_EQUAL(listener_a->last_event()->time(), 5);
	BOOST_CHECK_EQUAL(listener_b->last_event()->time(), 5);

	// all listeners world information is at t = 5
	BOOST_CHECK_EQUAL(listener_a->last_world_information().time(), 5);
	BOOST_CHECK_EQUAL(listener_b->last_world_information().time(), 5);

	// robot b is at (1.0, 0.5, 3.0)
	BOOST_CHECK_CLOSE(robot_data_b->position()(kXCoord), 1.0, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(kYCoord), 0.5, 0.1);
	BOOST_CHECK_CLOSE(robot_data_b->position()(kZCoord), 3.0, 0.1);
}


/*
 * Test: pass an event for t = 3 then another event for a past time ( t = 1 )
 * Fixture: SimpleWorldFixture
 * Expected Results: An invalid argument exception is thrown
 */
BOOST_FIXTURE_TEST_CASE(abstract_event_handler_past_event_test, SimpleWorldFixture) {
	// Test set up: EventHandler and Listeners
	boost::shared_ptr<AbstractViewFactory> view_factory;
	view_factory.reset(new ViewFactory<View> ());

	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	EventHandler event_handler(history, robot_control);


	boost::shared_ptr<TestSimulationListener> listener_a;
	listener_a.reset(new TestSimulationListener());

	boost::shared_ptr<TestSimulationListener> listener_b;
	listener_b.reset(new TestSimulationListener());

	event_handler.register_listener(listener_a);
	event_handler.register_listener(listener_b);

	// Test set up: Constructing first event and passing it to the handler
	boost::shared_ptr<LookEvent> look_event;
	look_event.reset(new LookEvent(3));
	boost::shared_ptr<TimePoint> time_point(new TimePoint());
	event_handler.handle_event(look_event, *time_point);
	history->insert(time_point);

	// constructing the second event for past time t = 1
	boost::shared_ptr<LookEvent> look_event_b;
	look_event_b.reset(new LookEvent(1));

	// Expect disappointment
	boost::shared_ptr<TimePoint> time_point_b(new TimePoint());
	BOOST_CHECK_THROW(event_handler.handle_event(look_event_b, *time_point_b), std::invalid_argument);
}

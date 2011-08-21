/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
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
 * factories_test.cc
 *
 *  Created on: Feb 24, 2009
 *      Author: craupach
 */

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include "../../SimulationKernel/factories.h"
#include "../Fixtures/simple_world_fixture.h"
#include "../../ViewModels/global_view.h"
#include "../../Views/view_factory.h"
#include "../../SimulationKernel/uniform_robot_control.h"

BOOST_FIXTURE_TEST_CASE(EventHandlerFactoryTest, SimpleWorldFixture) {
	// RobotControl needed for EventHandler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<GlobalView>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	//TODO asetzer: this unit test must be redone
	
	/*std::map<std::string, std::string> parameter_map;

	// create map with minimal required parameters for event handler which ONLY handles MarkerRequests
	// (with the standard request handler)
	parameter_map["MARKER_REQUEST_HANDLER_TYPE"] = std::string("STANDARD");
	parameter_map["STANDARD_MARKER_REQUEST_HANDLER_DISCARD_PROB"] = std::string("1.0");
	parameter_map["STANDARD_MARKER_REQUEST_HANDLER_SEED"] = "42";

	// NO other request handlers
	parameter_map["TYPE_CHANGE_REQUEST_HANDLER_TYPE"] = std::string("NONE");
	parameter_map["POSITION_REQUEST_HANDLER_TYPE"] = std::string("NONE");
	parameter_map["VELOCITY_REQUEST_HANDLER_TYPE"] = std::string("NONE");
	parameter_map["ACCELERATION_REQUEST_HANDLER_TYPE"] = std::string("NONE");

	// should be constructed normally
	boost::shared_ptr<EventHandler> event_handler;
	BOOST_CHECK_NO_THROW(event_handler = Factory::event_handler_factory(parameter_map, history, robot_control));
	BOOST_CHECK(event_handler);
	BOOST_CHECK(event_handler->marker_request_handler_);
	//BOOST_CHECK_EQUAL(1.0, event_handler->marker_request_handler_->discard_probability_);
	*/
}



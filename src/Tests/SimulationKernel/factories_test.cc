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

	std::map<std::string, std::string> parameter_map;

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
}



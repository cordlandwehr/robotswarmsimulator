/*
 * basic_edge_test.cc
 *
 *  Created on: 03.08.2011
 *      Author: Jonas
 */

#include <boost/test/unit_test.hpp>

#include "../../Model/robot_data.h"
#include "../../Model/edge.h"
#include "../../Model/robot_identifier.h"

#include "../../RobotImplementations/simple_robot.h"

#include "../../EventHandlers/event_handler.h"
#include "../../EventHandlers/vector_request_handler.h"
#include "../../EventHandlers/marker_request_handler.h"
#include "../../EventHandlers/type_change_request_handler.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_graph_fixture.h"



using namespace std;

BOOST_AUTO_TEST_CASE(basic_edge_test){
	// generate robot 0
	boost::shared_ptr<RobotIdentifier> rid0 (new RobotIdentifier(0));
	const SimpleRobot r0 (rid0);
	RobotData rd0 (rid0, boost::shared_ptr<Vector3d>(new Vector3d()), r0);

	// generate robot 1
	boost::shared_ptr<RobotIdentifier> rid1 (new RobotIdentifier(1));
	const SimpleRobot r1 (rid1);
	RobotData rd1 (rid1, boost::shared_ptr<Vector3d>(new Vector3d()), r1);

	// generate edge 0-1
	Edge e (rid0, rid1);

	// check for adding and removing edge from robot
	BOOST_CHECK_EQUAL(rd0.get_edges().size(), 0);
	rd0.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e.id()));
	BOOST_CHECK_EQUAL(rd0.get_edges().size(), 1);
	rd0.remove_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e.id()));
	BOOST_CHECK_EQUAL(rd0.get_edges().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(edge_view_test, SimpleGraphFixture) {
	// setup of event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<View>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));
	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<VectorRequestHandler> request_handler_acc(new VectorRequestHandler(5, 0.0, *history));
	event_handler.set_acceleration_request_handler(request_handler_acc);

	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(5, 0.0, *history));
	event_handler.set_velocity_request_handler(request_handler_vel);

	boost::shared_ptr<VectorRequestHandler> request_handler_pos(new VectorRequestHandler(5, 0.0, *history));
	event_handler.set_position_request_handler(request_handler_pos);

	boost::shared_ptr<MarkerRequestHandler> request_handler_marker(new MarkerRequestHandler(5, 0.0, *history));
	event_handler.set_marker_request_handler(request_handler_marker);

	boost::shared_ptr<TypeChangeRequestHandler> request_handler_type(new TypeChangeRequestHandler(5, 0.0, *history));
	event_handler.set_type_change_request_handler(request_handler_type);

	// checking edge view
	const RobotData& rd_a = history->get_newest().world_information().get_according_robot_data(robot_a->id());
//	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a.robot()).get_neighbors().size(), 1);
//	BOOST_CHECK_EQUAL(robot_a->get_neighbors()[0], id_b);
}

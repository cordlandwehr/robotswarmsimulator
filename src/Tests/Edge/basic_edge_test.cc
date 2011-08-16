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
#include "../../EventHandlers/insert_edge_request_handler.h"
#include "../../EventHandlers/remove_edge_request_handler.h"

#include "../../Requests/insert_edge_request.h"
#include "../../Requests/remove_edge_request.h"

#include "../../SimulationKernel/uniform_robot_control.h"

//#include "../Utilities/distribution_generator.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_graph_fixture.h"

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
	// setup of view and event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<LocalGraphView>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	robot_control->compute_view(*robot_a);
	robot_control->compute_view(*robot_b);
	robot_control->compute_view(*robot_c);

	EventHandler event_handler(history, robot_control);

	//boost::shared_ptr<DistributionGenerator> generator(new DistributionGenerator(0));
	//View::set_distribution_generator(generator);

	// checking edge view neighbors
	const RobotData& rd_a = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a.robot()).get_neighbors().size(), 0);

	const RobotData& rd_b = history->get_newest().world_information().get_according_robot_data(robot_b->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_b.robot()).get_neighbors().size(), 2);

	const RobotData& rd_c = history->get_newest().world_information().get_according_robot_data(robot_c->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_neighbors().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_neighbors()[0], id_b);

	// checking edge view edges
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a.robot()).get_edges().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a.robot()).get_edges()[0], edge_ab->id());

	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_b.robot()).get_edges().size(), 2);

	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_edges().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_edges()[0], edge_bc->id());
}

BOOST_FIXTURE_TEST_CASE(edge_handler_test, SimpleGraphFixture) {
	// setup of view and event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<LocalGraphView>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	EventHandler event_handler(history, robot_control);

	//boost::shared_ptr<DistributionGenerator> generator(new DistributionGenerator(0));
	//View::set_distribution_generator(generator);

	boost::shared_ptr<InsertEdgeRequestHandler> request_handler_insert_edge(new InsertEdgeRequestHandler(5, 0.0, *history));
	event_handler.set_insert_edge_request_handler(request_handler_insert_edge);

	boost::shared_ptr<RemoveEdgeRequestHandler> request_handler_remove_edge(new RemoveEdgeRequestHandler(5, 0.0, *history));
	event_handler.set_remove_edge_request_handler(request_handler_remove_edge);

	// generate requests
	boost::shared_ptr<Edge> new_edge(new DirectedEdge(id_a, id_b));
	boost::shared_ptr<InsertEdgeRequest> insert_edge_request(new InsertEdgeRequest(*robot_b, new_edge));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(1));
	handle_requests_event->add_to_requests(insert_edge_request);

	// handling the event
	boost::shared_ptr<TimePoint> time_point(new TimePoint());
	event_handler.handle_event(handle_requests_event, *time_point);
	history->insert(time_point);

	// generate look event
	boost::shared_ptr<LookEvent> look_event(new LookEvent(2));
	look_event->add_to_robot_subset(robot_a);
	look_event->add_to_robot_subset(robot_b);
	look_event->add_to_robot_subset(robot_c);
	time_point.reset(new TimePoint());
	event_handler.handle_event(look_event, *time_point);
	history->insert(time_point);

	// checking edge view neighbors
	const RobotData& rd_a = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a.robot()).get_neighbors().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a.robot()).get_neighbors()[0], id_b);

	const RobotData& rd_b = history->get_newest().world_information().get_according_robot_data(robot_b->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_b.robot()).get_neighbors().size(), 2);

	const RobotData& rd_c = history->get_newest().world_information().get_according_robot_data(robot_c->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_neighbors().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_neighbors()[0], id_b);

	// checking edge view edges
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a.robot()).get_edges().size(), 2);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_b.robot()).get_edges().size(), 3);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_edges().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c.robot()).get_edges()[0], edge_bc->id());

	// generate requests
	boost::shared_ptr<RemoveEdgeRequest> remove_edge_request(new RemoveEdgeRequest(*robot_b, edge_bc));

	// construction of handle_requests_event
	handle_requests_event.reset(new HandleRequestsEvent(3));
	handle_requests_event->add_to_requests(remove_edge_request);

	// handling the event
	time_point.reset(new TimePoint());
	event_handler.handle_event(handle_requests_event, *time_point);
	history->insert(time_point);

	// generate look event
	look_event.reset(new LookEvent(4));
	look_event->add_to_robot_subset(robot_a);
	look_event->add_to_robot_subset(robot_b);
	look_event->add_to_robot_subset(robot_c);
	time_point.reset(new TimePoint());
	event_handler.handle_event(look_event, *time_point);
	history->insert(time_point);

	// checking edge view neighbors
	const RobotData& rd_a_2 = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a_2.robot()).get_neighbors().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a_2.robot()).get_neighbors()[0], id_b);

	const RobotData& rd_b_2 = history->get_newest().world_information().get_according_robot_data(robot_b->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_b_2.robot()).get_neighbors().size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_b_2.robot()).get_neighbors()[0], id_a);

	const RobotData& rd_c_2 = history->get_newest().world_information().get_according_robot_data(robot_c->id());
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c_2.robot()).get_neighbors().size(), 0);

	// checking edge view edges
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_a_2.robot()).get_edges().size(), 2);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_b_2.robot()).get_edges().size(), 2);
	BOOST_CHECK_EQUAL(dynamic_cast<const SimpleGraphTestRobot&>(rd_c_2.robot()).get_edges().size(), 0);
}

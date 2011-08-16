/*
 * message_test.cc
 *
 *  Created on: 10.08.2011
 *      Author: Jonas
 */

#include <boost/test/unit_test.hpp>

#include "../../Model/robot_data.h"
#include "../../Model/edge.h"
#include "../../Model/message_identifier.h"
#include "../../Model/message.h"

#include "../../RobotImplementations/simple_robot.h"

#include "../../EventHandlers/event_handler.h"
#include "../../EventHandlers/remove_edge_request_handler.h"
#include "../../EventHandlers/message_request_handler.h"

#include "../../Requests/remove_edge_request.h"
#include "../../Requests/message_request.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../Fixtures/simple_graph_fixture.h"

BOOST_FIXTURE_TEST_CASE(message_handler_test, SimpleGraphFixture) {
	// setup of view and event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<LocalGraphView>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));

	EventHandler event_handler(history, robot_control);

	boost::shared_ptr<DistributionGenerator> generator(new DistributionGenerator(0));
	//View::set_distribution_generator(generator);

	boost::shared_ptr<MessageRequestHandler> message_request_handler(new MessageRequestHandler(5, 0.0, *history));
	event_handler.set_message_request_handler(message_request_handler);

	boost::shared_ptr<RemoveEdgeRequestHandler> request_handler_remove_edge(new RemoveEdgeRequestHandler(5, 0.0, *history));
	event_handler.set_remove_edge_request_handler(request_handler_remove_edge);

	// generate requests
	boost::shared_ptr<Identifier> message_ba_id(new MessageIdentifier());
	boost::shared_ptr<Vector3d> message_pos(new Vector3d());
	boost::shared_ptr<Message> message_ba(new Message(message_ba_id, message_pos, id_b, id_a));
	boost::shared_ptr<MessageRequest> message_request_ba(new MessageRequest(*robot_b, message_ba));

	boost::shared_ptr<Identifier> message_bc_id(new MessageIdentifier());
	boost::shared_ptr<Message> message_bc(new Message(message_bc_id, message_pos, id_b, id_c));
	boost::shared_ptr<MessageRequest> message_request_bc(new MessageRequest(*robot_b, message_bc));

	boost::shared_ptr<Identifier> message_ca_id(new MessageIdentifier());
	boost::shared_ptr<Message> message_ca(new Message(message_ca_id, message_pos, id_c, id_a));
	boost::shared_ptr<MessageRequest> message_request_ca(new MessageRequest(*robot_c, message_ca));

	boost::shared_ptr<RemoveEdgeRequest> remove_edge_request(new RemoveEdgeRequest(*robot_b, edge_bc));

	// construction of handle_requests_event
	boost::shared_ptr<HandleRequestsEvent> handle_requests_event(new HandleRequestsEvent(1));
	handle_requests_event->add_to_requests(remove_edge_request);
	handle_requests_event->add_to_requests(message_request_ba);
	handle_requests_event->add_to_requests(message_request_bc);
	handle_requests_event->add_to_requests(message_request_ca);

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

	const RobotData& rd_a = history->get_newest().world_information().get_according_robot_data(robot_a->id());
	const RobotData& rd_b = history->get_newest().world_information().get_according_robot_data(robot_b->id());
	const RobotData& rd_c = history->get_newest().world_information().get_according_robot_data(robot_c->id());

	BOOST_CHECK_EQUAL(rd_a.get_number_of_messages(), 1);
	BOOST_CHECK_EQUAL(rd_a.get_message()->id(), message_ba_id);
	BOOST_CHECK_EQUAL(rd_b.get_number_of_messages(), 0);
	BOOST_CHECK_EQUAL(rd_c.get_number_of_messages(), 1);
	BOOST_CHECK_EQUAL(rd_c.get_message()->id(), message_bc_id);
	BOOST_CHECK_EQUAL(rd_c.last_request_successful(), false);
}

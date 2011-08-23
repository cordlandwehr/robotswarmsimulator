/*
 * simple_graph_fixture.h
 *
 *  Created on: 03.08.2011
 *      Author: Jonas
 */

#ifndef SIMPLE_GRAPH_FIXTURE_H_
#define SIMPLE_GRAPH_FIXTURE_H_

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <set>
#include <string>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Model/robot_identifier.h"
#include "../../Model/edge.h"
#include "../../Model/directed_edge.h"
#include "../../Model/undirected_edge.h"

#include "../../ViewModels/local_graph_view.h"

#include "../../Events/look_event.h"
#include "../../Events/event.h"

#include "../../SimulationControl/history.h"
#include "../../SimulationControl/time_point.h"

#include "../../EventHandlers/event_handler.h"

#include "../../Utilities/vector_arithmetics.h"



class SimpleGraphTestRobot : public Robot {
public:
	SimpleGraphTestRobot(boost::shared_ptr<RobotIdentifier> id)
		: Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute() {
		std::set<boost::shared_ptr<Request> > empty_set;
		return empty_set;
	}

	const std::vector<boost::shared_ptr<RobotIdentifier> > get_neighbors() const {
		return view_->get_visible_robots(*this);
	}

	const std::vector<boost::shared_ptr<EdgeIdentifier> > get_edges() const {
		return view_->get_visible_edges(*this);
	}

	bool is_edge_directed(boost::shared_ptr<EdgeIdentifier> e_id) const {
		return view_->is_edge_directed(e_id);
	}

	boost::shared_ptr<RobotIdentifier> get_edge_source(boost::shared_ptr<EdgeIdentifier> e_id) const {
		return view_->get_edge_source(e_id);
	}

	boost::shared_ptr<RobotIdentifier> get_edge_target(boost::shared_ptr<EdgeIdentifier> e_id) const {
		return view_->get_edge_target(e_id);
	}

	boost::shared_ptr<RobotIdentifier> get_sender(boost::shared_ptr<MessageIdentifier> m) const {
		return view_->get_sender(m);
	}

	virtual std::string get_algorithm_id () const {
		return "SimpleRobot";
	}
};

/**
 * This fixture creates a simple graph with a history (size 5) and a world information for time 0
 * which contains three robots a, b and c and edges from a to b and from b to c.
 *
 *                              a <-- b <-> c
 */
struct SimpleGraphFixture {
	SimpleGraphFixture() {
		BOOST_TEST_MESSAGE("setup simple graph fixture");

		initial_world_information.reset(new WorldInformation());
		history.reset(new History(5));

		// create Robotidentifier
		id_a.reset(new RobotIdentifier(0));
		id_b.reset(new RobotIdentifier(1));
		id_c.reset(new RobotIdentifier(2));

		// create Robot
		robot_a.reset(new SimpleGraphTestRobot(id_a));
		robot_b.reset(new SimpleGraphTestRobot(id_b));
		robot_c.reset(new SimpleGraphTestRobot(id_c));

		// create RobotData
		robot_data_a.reset(new RobotData(id_a, boost::shared_ptr<Vector3d>(new Vector3d()), *robot_a));
		robot_data_b.reset(new RobotData(id_b, boost::shared_ptr<Vector3d>(new Vector3d()), *robot_b));
		robot_data_c.reset(new RobotData(id_c, boost::shared_ptr<Vector3d>(new Vector3d()), *robot_c));

		// add all robots to the world information
		initial_world_information->add_robot_data(robot_data_a);
		initial_world_information->add_robot_data(robot_data_b);
		initial_world_information->add_robot_data(robot_data_c);

		// create two Edges
		edge_ab.reset(new DirectedEdge(id_b, id_a));
		initial_world_information->add_edge(edge_ab);
//		robot_data_a->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(edge_ab->id()));
//		robot_data_b->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(edge_ab->id()));

		edge_bc.reset(new UndirectedEdge(id_b, id_c));
		initial_world_information->add_edge(edge_bc);
//		robot_data_b->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(edge_bc->id()));
//		robot_data_c->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(edge_bc->id()));

		// velocity and acceleration needed for event step
		// create velocity for robot a: (0,0,0)
		boost::shared_ptr<Vector3d> vel_a(new Vector3d());
		vel_a->insert_element(kXCoord,0.0);
		vel_a->insert_element(kYCoord,0.0);
		vel_a->insert_element(kZCoord,0.0);
		robot_data_a->set_velocity(vel_a);

		// create acceleration for robot a: (0,0,0)
		boost::shared_ptr<Vector3d> acc_a(new Vector3d());
		acc_a->insert_element(kXCoord,0.0);
		acc_a->insert_element(kYCoord,0.0);
		acc_a->insert_element(kZCoord,0.0);
		robot_data_a->set_acceleration(acc_a);

		// create  velocity for robot b: (1,0,0)
		boost::shared_ptr<Vector3d> vel_b(new Vector3d());
		vel_b->insert_element(kXCoord,1.0);
		vel_b->insert_element(kYCoord,0.0);
		vel_b->insert_element(kZCoord,0.0);
		robot_data_b->set_velocity(vel_b);

		// create acceleration for robot b: (0,0,0)
		boost::shared_ptr<Vector3d> acc_b(new Vector3d());
		acc_b->insert_element(kXCoord,0.0);
		acc_b->insert_element(kYCoord,0.0);
		acc_b->insert_element(kZCoord,0.0);
		robot_data_b->set_acceleration(acc_b);

		// create velocity for robot c: (0,0,0)
		boost::shared_ptr<Vector3d> vel_c(new Vector3d());
		vel_c->insert_element(kXCoord,0.0);
		vel_c->insert_element(kYCoord,0.0);
		vel_c->insert_element(kZCoord,0.0);
		robot_data_c->set_velocity(vel_c);

		// create acceleration for robot c: (1.0, 1.0, 1.0)
		boost::shared_ptr<Vector3d> acc_c(new Vector3d());
		acc_c->insert_element(kXCoord,1.0);
		acc_c->insert_element(kYCoord,1.0);
		acc_c->insert_element(kZCoord,1.0);
		robot_data_c->set_acceleration(acc_c);

		/* create coordinate-axis for robot a: x-axis (1.0, 0.0, 0.0)
											   y-axis (0.0, 1.0, 0.0)
											   z-axis (0.0, 0.0, 1.0)*/
		boost::shared_ptr<Vector3d> axis_x_a(new Vector3d());
		boost::shared_ptr<Vector3d> axis_y_a(new Vector3d());
		boost::shared_ptr<Vector3d> axis_z_a(new Vector3d());
		boost::tuple< boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d> > axis_a(axis_x_a, axis_y_a, axis_z_a);
		boost::get<0>(axis_a)->insert_element(kXCoord, 1.0);
		boost::get<0>(axis_a)->insert_element(kYCoord, 0.0);
		boost::get<0>(axis_a)->insert_element(kZCoord, 0.0);
		boost::get<1>(axis_a)->insert_element(kXCoord, 0.0);
		boost::get<1>(axis_a)->insert_element(kYCoord, 1.0);
		boost::get<1>(axis_a)->insert_element(kZCoord, 0.0);
		boost::get<2>(axis_a)->insert_element(kXCoord, 0.0);
		boost::get<2>(axis_a)->insert_element(kYCoord, 0.0);
		boost::get<2>(axis_a)->insert_element(kZCoord, 1.0);
		robot_data_a->set_coordinate_system_axis(axis_a);

		/* create coordinate-axis for robot b: x-axis (-1.0, 0.0, 0.0)
											   y-axis (0.0, 0.5, 0.0)
											   z-axis (0.0, 0.0, 2.0)*/
		boost::shared_ptr<Vector3d> axis_x_b(new Vector3d());
		boost::shared_ptr<Vector3d> axis_y_b(new Vector3d());
		boost::shared_ptr<Vector3d> axis_z_b(new Vector3d());
		boost::tuple< boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d> > axis_b(axis_x_b, axis_y_b, axis_z_b);
		boost::get<0>(axis_b)->insert_element(kXCoord,-1.0);
		boost::get<0>(axis_b)->insert_element(kYCoord, 0.0);
		boost::get<0>(axis_b)->insert_element(kZCoord, 0.0);
		boost::get<1>(axis_b)->insert_element(kXCoord, 0.0);
		boost::get<1>(axis_b)->insert_element(kYCoord, 0.5);
		boost::get<1>(axis_b)->insert_element(kZCoord, 0.0);
		boost::get<2>(axis_b)->insert_element(kXCoord, 0.0);
		boost::get<2>(axis_b)->insert_element(kYCoord, 0.0);
		boost::get<2>(axis_b)->insert_element(kZCoord, 2.0);
		robot_data_b->set_coordinate_system_axis(axis_b);


		/* create coordinate-axis for robot c: x-axis (0.0, 0.5, 1.0)
											   y-axis (0.5, 0.0, -1.0)
											   z-axis (0.5, 0.5, 0.0)*/
		boost::shared_ptr<Vector3d> axis_x_c(new Vector3d());
		boost::shared_ptr<Vector3d> axis_y_c(new Vector3d());
		boost::shared_ptr<Vector3d> axis_z_c(new Vector3d());
		boost::tuple< boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d> > axis_c(axis_x_c, axis_y_c, axis_z_c);
		boost::get<0>(axis_c)->insert_element(kXCoord, 0.0);
		boost::get<0>(axis_c)->insert_element(kYCoord, 0.5);
		boost::get<0>(axis_c)->insert_element(kZCoord, 1.0);
		boost::get<1>(axis_c)->insert_element(kXCoord, 0.5);
		boost::get<1>(axis_c)->insert_element(kYCoord, 0.0);
		boost::get<1>(axis_c)->insert_element(kZCoord,-1.0);
		boost::get<2>(axis_c)->insert_element(kXCoord, 0.5);
		boost::get<2>(axis_c)->insert_element(kYCoord, 0.5);
		boost::get<2>(axis_c)->insert_element(kZCoord, 0.0);
		robot_data_c->set_coordinate_system_axis(axis_c);



		// set time of inital world information
		initial_world_information->set_time(0);

		// insert initial world information into history
		boost::shared_ptr<TimePoint> new_time_point(new TimePoint());
		new_time_point->set_world_information(initial_world_information);
		history->insert(new_time_point);

	}

	boost::shared_ptr<WorldInformation> initial_world_information;
	boost::shared_ptr<History> history;

	// Robot Datas
	boost::shared_ptr<RobotData> robot_data_a;
	boost::shared_ptr<RobotData> robot_data_b;
	boost::shared_ptr<RobotData> robot_data_c;

	// Robots
	boost::shared_ptr<SimpleGraphTestRobot> robot_a;
	boost::shared_ptr<SimpleGraphTestRobot> robot_b;
	boost::shared_ptr<SimpleGraphTestRobot> robot_c;

	// Robot Identifiers
	boost::shared_ptr<RobotIdentifier> id_a;
	boost::shared_ptr<RobotIdentifier> id_b;
	boost::shared_ptr<RobotIdentifier> id_c;

	// Edges
	boost::shared_ptr<Edge> edge_ab;
	boost::shared_ptr<Edge> edge_bc;
};

#endif /* SIMPLE_GRAPH_FIXTURE_H_ */

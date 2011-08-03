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
#include "../../Model/Edge.h"
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

	virtual std::string get_algorithm_id () const {
		return "SimpleRobot";
	}
};

/**
 * This fixture creates a simple graph with a history (size 5) and a world information for time 0
 * which contains three robots a, b and c and edges from a to b and from b to c.
 *
 *                              a --- b --- c
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
		boost::shared_ptr<Edge> e_ab (new Edge(id_a, id_b));
		initial_world_information->add_edge(e_ab);
		robot_data_a->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e_ab->id()));
		robot_data_b->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e_ab->id()));

		boost::shared_ptr<Edge> e_bc (new Edge(id_b, id_c));
		initial_world_information->add_edge(e_bc);
		robot_data_b->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e_bc->id()));
		robot_data_c->add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e_bc->id()));

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
};

#endif /* SIMPLE_GRAPH_FIXTURE_H_ */

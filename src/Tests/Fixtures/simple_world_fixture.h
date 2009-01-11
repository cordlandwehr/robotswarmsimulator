#ifndef SIMPLE_WORLD_FIXTURE_H_
#define SIMPLE_WORLD_FIXTURE_H_

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Events/look_event.h"
#include "../../Events/event.h"
#include "../../SimulationControl/history.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector3d.h"

/**
 * This fixture creates a simple world with a history (size 5) and a world information for time 0
 * which contains two robots:
 *
 * robot a at (0.0, 0.0, 0.0) with zero acceleration and velocity
 *
 * robot b at (1.0, 0.5, 3.0) with zero acceleration and velocity (1.0, 0.0, 0.0)
 *
 */
struct SimpleWorldFixture {
	SimpleWorldFixture() {
		BOOST_TEST_MESSAGE("setup simple world fixture");

		initial_world_information.reset(new WorldInformation());
		history.reset(new History(5));

		robot_a.reset(new Robot());
		robot_b.reset(new Robot());
		robot_data_a.reset(new RobotData(*robot_a));
		robot_data_b.reset(new RobotData(*robot_b));

		// create position for robot a: (0,0,0)
		Vector3d * pos_a_ptr = new Vector3d;
		boost::shared_ptr<Vector3d> pos_a;
		pos_a.reset(pos_a_ptr);
		pos_a->insert_element(0,0.0);
		pos_a->insert_element(1,0.0);
		pos_a->insert_element(2,0.0);
		robot_data_a->set_position(pos_a);

		// create velocity for robot a: (0,0,0)
		Vector3d * vel_a_ptr = new Vector3d;
		boost::shared_ptr<Vector3d> vel_a;
		vel_a.reset(vel_a_ptr);
		vel_a->insert_element(0,0.0);
		vel_a->insert_element(1,0.0);
		vel_a->insert_element(2,0.0);
		robot_data_a->set_velocity(vel_a);

		// create acceleration for robot a: (0,0,0)
		Vector3d * acc_a_ptr = new Vector3d;
		boost::shared_ptr<Vector3d> acc_a;
		acc_a.reset(acc_a_ptr);
		acc_a->insert_element(0,0.0);
		acc_a->insert_element(0,0.0);
		acc_a->insert_element(0,0.0);
		robot_data_a->set_acceleration(acc_a);

		// create position for robot b: (1,0.5,3)
		Vector3d * pos_b_ptr = new Vector3d;
		boost::shared_ptr<Vector3d> pos_b;
		pos_b.reset(pos_b_ptr);
		pos_b->insert_element(0,1.0);
		pos_b->insert_element(1,0.5);
		pos_b->insert_element(2,3.0);
		robot_data_b->set_position(pos_b);

		// create  velocity for robot b: (1,0,0)
		Vector3d * vel_b_ptr = new Vector3d;
		boost::shared_ptr<Vector3d> vel_b;
		vel_b.reset(vel_b_ptr);
		vel_b->insert_element(0,1.0);
		vel_b->insert_element(1,0.0);
		vel_b->insert_element(2,0.0);
		robot_data_b->set_velocity(vel_b);

		// create acceleration for robot b: (0,0,0)
		Vector3d * acc_b_ptr = new Vector3d;
		boost::shared_ptr<Vector3d> acc_b;
		acc_b.reset(acc_b_ptr);
		acc_b->insert_element(0,0.0);
		acc_b->insert_element(1,0.0);
		acc_b->insert_element(2,0.0);
		robot_data_b->set_acceleration(acc_b);


		// add both robots to the world information
		initial_world_information->add_robot_data(robot_data_a);
		initial_world_information->add_robot_data(robot_data_b);

		// set time of inital world information
		initial_world_information->set_time(0);

		// insert initial world information into history
		history->push_back(initial_world_information);

	}

	boost::shared_ptr<WorldInformation> initial_world_information;
	boost::shared_ptr<History> history;

	boost::shared_ptr<RobotData> robot_data_a;
	boost::shared_ptr<RobotData> robot_data_b;
	boost::shared_ptr<Robot> robot_a;
	boost::shared_ptr<Robot> robot_b;

};

#endif /* SIMPLE_WORLD_FIXTURE_H_ */
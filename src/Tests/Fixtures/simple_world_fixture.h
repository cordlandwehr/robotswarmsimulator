/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>

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

#ifndef SIMPLE_WORLD_FIXTURE_H_
#define SIMPLE_WORLD_FIXTURE_H_

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
#include "../../Model/sphere_identifier.h"
#include "../../Model/sphere.h"
#include "../../Model/box.h"
#include "../../Model/box_identifier.h"
#include "../../Model/marker_identifier.h"
#include "../../Events/look_event.h"
#include "../../Events/event.h"
#include "../../SimulationControl/history.h"
#include "../../SimulationControl/time_point.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector_arithmetics.h"



class SimpleRobot : public Robot {
public:
	SimpleRobot(boost::shared_ptr<RobotIdentifier> id)
		: Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute() {
		std::set<boost::shared_ptr<Request> > empty_set;
		return empty_set;
	}

	virtual std::string get_algorithm_id () const {
		return "SimpleRobot";
	}
};

/**
 * This fixture creates a simple world with a history (size 5) and a world information for time 0
 * which contains three robots:
 *
 * robot a at (0.0, 0.0, 0.0) with zero acceleration and velocity, master, ready
 *
 * robot b at (1.0, 0.5, 3.0) with zero acceleration and velocity (1.0, 0.0, 0.0), slave, ready
 *
 * robot c at (1.0, 1.0, 1.0) with acceleration (1.0, 1.0, 1.0) and zero velocity, slave, sleeping
 *
 * All robots have different coordinate systems
 *
 * additionally, there is a sphere at (-3.0, -3.0, 0.0) with radius 1.0 and a
 * box at (1.0, -3.0, -5.0) with hight=depth=width=1.0.
 */
struct SimpleWorldFixture {
	SimpleWorldFixture() {
		BOOST_TEST_MESSAGE("setup simple world fixture");

		initial_world_information.reset(new WorldInformation());
		history.reset(new History(5));

		// create Robotidentifier
		id_a.reset(new RobotIdentifier(0));
		id_b.reset(new RobotIdentifier(1));
		id_c.reset(new RobotIdentifier(2));

		// collect robots to vector robots
		robot_a.reset(new SimpleRobot(id_a));
		robot_b.reset(new SimpleRobot(id_b));
		robot_c.reset(new SimpleRobot(id_c));

		robots.push_back(robot_a);
		robots.push_back(robot_b);
		robots.push_back(robot_c);

		// create position for robot a: (0,0,0)
		boost::shared_ptr<Vector3d> pos_a;
		pos_a.reset(new Vector3d);
		pos_a->insert_element(kXCoord,0.0);
		pos_a->insert_element(kYCoord,0.0);
		pos_a->insert_element(kZCoord,0.0);
		robot_data_a.reset(new RobotData(id_a, pos_a, robot_a));

		// create position for robot b: (1,0.5,3)
		Vector3d * pos_b_ptr = new Vector3d;
		boost::shared_ptr<Vector3d> pos_b;
		pos_b.reset(pos_b_ptr);
		pos_b->insert_element(kXCoord,1.0);
		pos_b->insert_element(kYCoord,0.5);
		pos_b->insert_element(kZCoord,3.0);
		robot_data_b.reset(new RobotData(id_b, pos_b, robot_b));

		// create position for robot c: (1.0, 1.0, 1.0)
		boost::shared_ptr<Vector3d> pos_c;
		pos_c.reset(new Vector3d());
		pos_c->insert_element(kXCoord,1.0);
		pos_c->insert_element(kYCoord,1.0);
		pos_c->insert_element(kZCoord,1.0);
		robot_data_c.reset(new RobotData(id_c, pos_c, robot_c));


		// set types for robots
		RobotType robtype_a = MASTER;
		robot_data_a->set_type(robtype_a);
		RobotType robtype_b = SLAVE;
		robot_data_b->set_type(robtype_b);
		RobotType robtype_c = SLAVE;
		robot_data_c->set_type(robtype_c);

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

		// set stati for robots
		RobotStatus robstatus_a = READY;
		robot_data_a->set_status(robstatus_a);
		RobotStatus robstatus_b = READY;
		robot_data_b->set_status(robstatus_b);
		RobotStatus robstatus_c = SLEEPING;
		robot_data_c->set_status(robstatus_c);

		//TODO: set markerinformation, algorithm and color!

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

		// create Obstacleidentifier
		id_sphere.reset(new SphereIdentifier(0));
		id_box.reset(new BoxIdentifier(1));
		//TODO(mmarcus) not needed(?)
		//id_marker.reset(new MarkerIdentifier(2));

		// create position for the sphere: (-3.0, -3.0, 0.0)
		boost::shared_ptr<Vector3d> pos_sphere;
		pos_sphere.reset(new Vector3d());
		pos_sphere->insert_element(kXCoord,-3.0);
		pos_sphere->insert_element(kYCoord,-3.0);
		pos_sphere->insert_element(kZCoord, 0.0);

		// create sphere with radius 1.0
		sphere.reset(new Sphere(id_sphere, pos_sphere, 1.0));

		// create position for the box: (1.0, -3.0, -5.0)
		boost::shared_ptr<Vector3d> pos_box;
		pos_box.reset(new Vector3d());
		pos_box->insert_element(kXCoord, 1.0);
		pos_box->insert_element(kYCoord,-3.0);
		pos_box->insert_element(kZCoord,-5.0);

		// create box with depth x width x height = 1x1x1
		box.reset(new Box(id_box, pos_box, 1.0, 1.0, 1.0));

		//TODO(mmarcus) setting position for marker!
		// create position for the marker: (0.0, -1.0, 0.0)
		/*boost::shared_ptr<Vector3d> pos_marker;
		pos_marker.reset(new Vector3d());
		pos_marker->insert_element(kXCoord, 0.0);
		pos_marker->insert_element(kYCoord,-1.0);
		pos_marker->insert_element(kZCoord, 0.0);*/

		//TODO(mmarcus) create marker

		// add all robots to the world information
		initial_world_information->add_robot_data(robot_data_a);
		initial_world_information->add_robot_data(robot_data_b);
		initial_world_information->add_robot_data(robot_data_c);

		// add obstacles to the world information
		initial_world_information->add_obstacle(sphere);
		initial_world_information->add_obstacle(box);

		//TODO(mmarcus) add marker to the world information

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
	boost::shared_ptr<Robot> robot_a;
	boost::shared_ptr<Robot> robot_b;
	boost::shared_ptr<Robot> robot_c;

	// Robot Identifiers
	boost::shared_ptr<RobotIdentifier> id_a;
	boost::shared_ptr<RobotIdentifier> id_b;
	boost::shared_ptr<RobotIdentifier> id_c;

	std::vector<boost::shared_ptr<Robot> > robots;

	// Obstacle Identifiers
	boost::shared_ptr<SphereIdentifier> id_sphere;
	boost::shared_ptr<BoxIdentifier> id_box;
	//TODO (mmarcus) not needed(?)
	//boost::shared_ptr<MarkerIdentifier> id_marker;

	// Obstacles
	boost::shared_ptr<Sphere> sphere;
	boost::shared_ptr<Box> box;

	//TODO (mmarcus) make marker!
};

#endif /* SIMPLE_WORLD_FIXTURE_H_ */

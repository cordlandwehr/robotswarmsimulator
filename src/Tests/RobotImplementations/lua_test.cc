/*
 * lua_test.cc
 *
 *  Created on: 07.02.2009
 *      Author: Daniel
 */
#include <boost/test/unit_test.hpp>

#include "../../RobotImplementations/lua_robot.h"
#include "../../Views/abstract_view_factory.h"
#include "../../Views/global_view.h"
#include "../../Views/view_factory.h"

#include "../../Tests/Fixtures/simple_world_fixture.h"


BOOST_FIXTURE_TEST_CASE(lua_test, SimpleWorldFixture) {
	boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(3));
	boost::shared_ptr<std::string> lua_file(new std::string("src/Tests/RobotImplementations/test.lua"));
	boost::shared_ptr<LuaRobot> robot(new LuaRobot(id, lua_file));
	boost::shared_ptr<Vector3d> robot_pos = boost::shared_ptr<Vector3d>(new Vector3d());
	robot_pos->insert_element(kXCoord,1);
	robot_pos->insert_element(kYCoord,2);
	robot_pos->insert_element(kZCoord,3);
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

	boost::shared_ptr<RobotData> robot_data = boost::shared_ptr<RobotData>(new RobotData(id, robot_pos, *robot));
	robot_data->set_coordinate_system_axis(axis_a);

	robots.push_back(robot);
	initial_world_information->add_robot_data(robot_data);

	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<GlobalView>());
	boost::shared_ptr<View> view(view_factory->create_new_view_instance(*initial_world_information));
	robot->set_view(view);
	robot->compute();
	robot->compute();

	//exit(1);
}

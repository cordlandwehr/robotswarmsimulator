/*
 * lua_robot.cc
 *
 *  Created on: 07.02.2009
 *      Author: Daniel
 */

#include "lua_robot.h"

#include <iostream>
#include <boost/foreach.hpp>

#include "../../Model/robot_identifier.h"
#include "../../Views/view.h"

namespace {
	boost::shared_ptr<View> view; //current view for the lua script
	const Robot* robot; //needed as "caller" in most view methods

	//TODO: int is dummy type.
	const std::vector<int> get_visible_robots() {
		std::set<boost::shared_ptr<RobotIdentifier> > robots = view->get_visible_robots(*robot);
		std::vector<int> result;
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> robot, robots) {
			//TODO: somehow convert identifier -> supported_lua_type
			result.push_back((int)robot.get()); //dummy line
		}
		return result;
	}
}

void LuaRobot::report_errors(int status) {
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(lua_state_.get(), -1) << std::endl;
		lua_pop(lua_state_.get(), 1); // remove error message
	}
}

LuaRobot::~LuaRobot() {

}

LuaRobot::LuaRobot(boost::shared_ptr<RobotIdentifier> id, const std::string& lua_file_name)
                   : Robot(id), lua_state_(lua_open(), lua_close) {
	luaL_openlibs(lua_state_.get());
	luabind::open(lua_state_.get());
	int status = luaL_loadfile(lua_state_.get(), lua_file_name.c_str());
	if(status != 0) {
		report_errors(status);
		//TODO: throw some exception
	}
}

std::set<boost::shared_ptr<Request> > LuaRobot::compute() {
	//register view methods to lua
	luabind::module(lua_state_.get())
	[
		 luabind::def("get_visible_robots", &get_visible_robots, luabind::copy_table(luabind::result))
	];

	if(view = view_.lock()) {
		robot = this;

		int status = lua_pcall(lua_state_.get(), 0, LUA_MULTRET, 0);
		report_errors(status);
	}
	else {
		//TODO: throw some exception
	}
	//TODO: somehow gather requests
	return std::set<boost::shared_ptr<Request> >();
}



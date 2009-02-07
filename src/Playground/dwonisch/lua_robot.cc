/*
 * lua_robot.cc
 *
 *  Created on: 07.02.2009
 *      Author: Daniel
 */

#include "lua_robot.h"

#include <iostream>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <deque>
#include <algorithm>

#include "../../Model/robot_identifier.h"
#include "../../Model/marker_identifier.h"
#include "../../Model/obstacle_identifier.h"
#include "../../Views/view.h"

namespace {
	boost::shared_ptr<View> view; //current view for the lua script
	const Robot* robot; //needed as "caller" in most view methods
	std::deque<boost::shared_ptr<Identifier> > queried_identifiers;

	template<typename T>
	const boost::shared_ptr<T> resolve(std::size_t identifier_index) {
		return boost::static_pointer_cast<T>(queried_identifiers[identifier_index]);
	}

	template<typename T>
	const std::vector<std::size_t> transform(const std::set<boost::shared_ptr<T> >& set) {
		std::size_t begin_index = queried_identifiers.size();
		queried_identifiers.insert(queried_identifiers.end(), set.begin(), set.end());
		std::vector<std::size_t> result(set.size());
		std::for_each(result.begin(), result.end(), boost::lambda::_1 = boost::lambda::var(begin_index)++);
		return result;
	}

	const std::vector<double> transform(const Vector3d& vec) {
		return std::vector<double>(vec.begin(), vec.end());
	}

	const std::vector<std::size_t> get_visible_robots() {
		return transform(view->get_visible_robots(*robot));
	}

	const std::vector<std::size_t> get_visible_obstacles() {
		return transform(view->get_visible_obstacles(*robot));
	}

	const std::vector<std::size_t> get_visible_markers() {
		return transform(view->get_visible_markers(*robot));
	}

	const std::vector<double> get_position(std::size_t index) {
		return transform(view->get_position(*robot, resolve<Identifier>(index)));
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
		 luabind::def("get_visible_robots", &get_visible_robots, luabind::copy_table(luabind::result)),
		 luabind::def("get_visible_obstacles", &get_visible_obstacles, luabind::copy_table(luabind::result)),
		 luabind::def("get_visible_markers", &get_visible_markers, luabind::copy_table(luabind::result)),
		 luabind::def("get_position", &get_position, luabind::copy_table(luabind::result))
	];

	if(view = view_.lock()) {
		robot = this;
		queried_identifiers.clear();

		int status = lua_pcall(lua_state_.get(), 0, LUA_MULTRET, 0);
		report_errors(status);
	}
	else {
		//TODO: throw some exception
	}
	//TODO: somehow gather requests
	return std::set<boost::shared_ptr<Request> >();
}



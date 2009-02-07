/*
 * lua_robot.h
 *
 *  Created on: 07.02.2009
 *      Author: Daniel
 */

#ifndef LUA_ROBOT_H_
#define LUA_ROBOT_H_

#include <string>
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include "luabind/table_policy.hpp"
#include "../../Model/robot.h"



class LuaRobot: public Robot {
public:
	~LuaRobot();
	LuaRobot(boost::shared_ptr<RobotIdentifier> id, const std::string& lua_file_name);

	virtual std::set<boost::shared_ptr<Request> > compute();
private:
	void report_errors(int status);
private:
	boost::shared_ptr<lua_State> lua_state_;
};

#endif /* LUA_ROBOT_H_ */

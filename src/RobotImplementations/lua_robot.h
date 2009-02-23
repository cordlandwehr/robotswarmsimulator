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
#include "lualib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/table_policy.hpp>

#include "../Model/robot.h"


class LuaRobot: public Robot {
public:
	~LuaRobot();
	LuaRobot(boost::shared_ptr<RobotIdentifier> id, const std::string& lua_file_name);

	virtual std::set<boost::shared_ptr<Request> > compute();
	virtual std::string get_algorithm_id () const;
private:
	void report_errors(int status);
	void register_lua_methods();
private:
	boost::shared_ptr<lua_State> lua_state_;
	std::string lua_file_name_;
};

#endif /* LUA_ROBOT_H_ */
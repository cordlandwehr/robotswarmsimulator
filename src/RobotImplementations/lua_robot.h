/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

/**
 * \brief RobotImplementation which uses a user defined lua script to perform compute steps.
 *
 * The lua script which should be used for the compute step can be defined in the constructor.
 * Note that the lua script must have a "main" method.
 */

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
	std::string lua_file_name_;
	boost::shared_ptr<lua_State> lua_state_;
};

#endif /* LUA_ROBOT_H_ */

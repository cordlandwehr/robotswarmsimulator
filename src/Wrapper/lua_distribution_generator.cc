/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 * distribution_generator_wrapper.cc
 *
 *  Created on: Jul 29, 2009
 *      Author: craupach
 */

#include "lua_distribution_generator.h"

DistributionGenerator lua_generator(0);

void LuaWrapper::lua_generator_set_seed(int seed) {
	lua_generator.set_seed(seed);
}

void LuaWrapper::lua_generator_init_uniform(int min, int max) {
	lua_generator.init_uniform(min, max);
}

int LuaWrapper::lua_generator_get_uniform() {
	return lua_generator.get_value_uniform();
}


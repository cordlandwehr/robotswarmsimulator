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
 * lua_distribution_generator.h
 *
 *  Created on: Jul 29, 2009
 *      Author: craupach
 */

#ifndef LUA_DISTRIBUTION_GENERATOR_H_
#define LUA_DISTRIBUTION_GENERATOR_H_

#include <Utilities/distribution_generator.h>

namespace LuaWrapper {

void lua_generator_set_seed(int seed);

void lua_generator_init_uniform(int min, int max);

int lua_generator_get_uniform();

}

#endif /* LUA_DISTRIBUTION_GENERATOR_H_ */

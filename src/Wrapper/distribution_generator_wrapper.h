/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 * distribution_generator_wrapper.h
 *
 *  Created on: May 03, 2009
 *      Author: peter
 *
 * Contains Lua wrappers for the distribution generator.
 * Although Lua already provides a random number generator, it is not as flexible as the boost equivalent. This wrappers
 * try to provide an easy possibility to access different random distributions (as for example a uniform distribution on
 * a unit sphere).
 *
 * Note: in fact, we have to wrap only one method, namely DistributionGenerator::get_value_uniform_on_sphere_3d(). All
 *       other methods can be exposed directly to Lua, but because this method returns a Vector3D (which is not
 *       exposed directly to Lua but wrapped itself into a Vector3dWrapper), we must provide a wrapping method
 *       explicitly.
 **/

#ifndef DISTRIBUTION_GENERATOR_WRAPPER_H_
#define DISTRIBUTION_GENERATOR_WRAPPER_H_

#include "vector_wrapper.h"

namespace LuaWrapper {
	/**
	 * DistributionGenerator::get_value_uniform_on_sphere_3d() must be wrapped because it returns a Vector3d, which is
	 * not directly exposed to Lua itself.
	 */
	const LuaWrapper::Vector3dWrapper get_value_uniform_on_sphere_3d(DistributionGenerator& distr_generator) {
		return transform(distr_generator.get_value_uniform_on_sphere_3d());
	}
}

#endif /* DISTRIBUTION_GENERATOR_WRAPPER_H_ */

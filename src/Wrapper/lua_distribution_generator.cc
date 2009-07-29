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


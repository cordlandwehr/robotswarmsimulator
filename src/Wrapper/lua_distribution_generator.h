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

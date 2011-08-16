//
//  lua_world_modifier.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 25.07.11.
//

#ifndef LUA_WORLD_MODIFIER_H_
#define LUA_WORLD_MODIFIER_H_

#include <boost/smart_ptr.hpp>
#include <string>
#include <set>

extern "C" {
#include "lualib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/table_policy.hpp>

#include "../Model/world_information.h"
#include "../Model/world_modifier.h"

class LuaWorldModifier : public WorldModifier {
public:
    LuaWorldModifier(const std::string& lua_file_name);
    virtual std::string get_algorithm_id() const;
    virtual std::set< boost::shared_ptr<Request> >
    compute(const boost::shared_ptr<WorldInformation> &world_information);
    
private:
	void report_errors(int status);
	void register_lua_methods();
    
	std::string lua_file_name_;
	boost::shared_ptr<lua_State> lua_state_;
};

#endif // LUA_WORLD_MODIFIER_H_
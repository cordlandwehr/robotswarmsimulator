//
//  lua_world_modifier.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 25.07.11.
//

#include <boost/foreach.hpp>

#include "../Model/world_information.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Requests/position_request.h"
#include "../Utilities/console_output.h"
#include "../Utilities/vector_arithmetics.h"

#include "lua_world_modifier.h"

LuaWorldModifier::LuaWorldModifier(const std::string& lua_file_name)
: lua_file_name_(lua_file_name), lua_state_(lua_open(), lua_close) {
	
    luaL_openlibs(lua_state_.get());
	luabind::open(lua_state_.get());
	
    register_lua_methods();
        
	int status = luaL_loadfile(lua_state_.get(), lua_file_name.c_str());
	
    if(status != 0) {
		report_errors(status);
		throw std::invalid_argument("Error while loading given lua file (" + lua_file_name + ").");
	}
	
    status = lua_pcall(lua_state_.get(), 0, LUA_MULTRET, 0);
    report_errors(status);
}

std::string LuaWorldModifier::get_algorithm_id() const{
    return "LUA_WORLD_MODIFIER";
}

std::set< boost::shared_ptr<Request> > 
LuaWorldModifier::compute(const boost::shared_ptr<WorldInformation> &world_information) {
    std::set< boost::shared_ptr<Request> > requests;
    
    try {
		luabind::call_function<void>(lua_state_.get(), "main");
	}
	catch(luabind::error& e) {
		luabind::object error_msg(luabind::from_stack(e.state(), -1));
	    std::cerr << error_msg << std::endl;
	}
    
    return requests;
}

void LuaWorldModifier::register_lua_methods() {
	//register view methods to lua
}

void LuaWorldModifier::report_errors(int status) {
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(lua_state_.get(), -1) << std::endl;
		lua_pop(lua_state_.get(), 1); // remove error message
	}
}


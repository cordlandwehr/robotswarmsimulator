//
//  marker_information_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 01.08.11.
//

#include "marker_information_wrapper.h"
#include <lua.h>

namespace LuaWrapper{
    boost::shared_ptr<lua_State> MarkerInformationWrapper::lua_state_;
    
    void MarkerInformationWrapper::set_lua_state (boost::shared_ptr < lua_State > state) {
	lua_state_ = state;
    }
    
    void MarkerInformationWrapper::add_data(const std::string& var_name, const luabind::object& object) {
	switch (luabind::type(object)) {
	  case LUA_TBOOLEAN:
	    marker_information_.add_data(var_name, luabind::object_cast<bool>(object));
	    break;
	  case LUA_TNUMBER:
	    marker_information_.add_data(var_name, luabind::object_cast<double>(object));
	    break;
	  case LUA_TSTRING:
	    marker_information_.add_data(var_name, luabind::object_cast<std::string>(object));
	    break;
	  default:
	    // TODO: Warning and/or exception.
	    break;
	}
    }
    
    luabind::object MarkerInformationWrapper::get_data(const std::string& var_name) {
	luabind::object new_object;
	boost::any value = marker_information_.get_data(var_name);
	
	if (value.type() == typeid(bool)) {
	  // cast to boolean
	  bool val = boost::any_cast<bool>(value);
	  new_object = luabind::object(lua_state_.get(), val);
	} else if (value.type() == typeid(double)
		  || value.type() == typeid(float)
		  || value.type() == typeid(int)) {
	  // cast to double
	  double val = boost::any_cast<double>(value);
	  new_object = luabind::object(lua_state_.get(), val);
	} else if (value.type() == typeid(std::string)) {
	  std::string val = boost::any_cast<std::string>(value);
	  new_object = luabind::object(lua_state_.get(), val);
	} else {
	  // unsupported type, return NIL object
	  new_object = luabind::object();
	}
	
	return new_object;
    }
   
    const std::vector<std::string> MarkerInformationWrapper::get_keys() {
      return marker_information_.get_keys();
    }
    
    void MarkerInformationWrapper::remove_data(const std::string& var_name) {
	marker_information_.remove_data(var_name);
    }
    
    const MarkerInformation& MarkerInformationWrapper::marker_information() const {
        return marker_information_;
    }
}
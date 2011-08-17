//
//  marker_information_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 01.08.11.
//

#include "marker_information_wrapper.h"

namespace LuaWrapper{
    void MarkerInformationWrapper::add_data(const std::string& var_name, const luabind::object& object) {
        marker_information_.add_data(var_name, object);
    }
    
    luabind::object MarkerInformationWrapper::get_data(const std::string& var_name) {
        return boost::any_cast<luabind::object>(marker_information_.get_data(var_name));
    }
   
    const std::vector<std::string> MarkerInformationWrapper::get_keys() {
      return marker_information_.get_keys();
    }
    
    void MarkerInformationWrapper::remove_data(const std::string& var_name) {
	marker_information_.remove_data(var_name);
    }
    
    MarkerInformation MarkerInformationWrapper::marker_information() const {
        return marker_information_;
    }
}
//
//  marker_information_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 01.08.11.
//

#ifndef MARKER_INFORMATION_WRAPPER_H_
#define MARKER_INFORMATION_WRAPPER_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>
#include <luabind/object.hpp>

#include "../Model/marker_information.h"

namespace LuaWrapper {
    class MarkerInformationWrapper {
    public:
        MarkerInformationWrapper() : marker_information_() {;}
        MarkerInformationWrapper(const MarkerInformation& marker_information) : marker_information_(marker_information) {;}
        
        void add_data(const std::string& var_name, const luabind::object& object);        
        luabind::object get_data(const std::string& var_name);   
	const std::vector<std::string> get_keys();
	bool has_key(const std::string& key);
	void remove_data(const std::string& var_name);
        const MarkerInformation& marker_information() const;
        static void set_lua_state(boost::shared_ptr<lua_State> state);
	
    private:
        MarkerInformation marker_information_;
	static boost::shared_ptr<lua_State> lua_state_;
	
    };
}

#endif // MARKER_INFORMATION_WRAPPER_H_
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
        
        /**
	 * Add or reset key value pair.
	 * 
	 * \param var_name	Name of the key.
	 * \param object	Value to be stored.
	 * 
	 * \warning		The implementation only supports int, float,
	 * 			double, bool and string values.
	 */
        void add_data(const std::string& var_name, const luabind::object& object);
	/**
	 * Returns the value for the given key.
	 * 
	 * \param var_name	Name of the key.
	 * \return		Value stored for the key wrapped as Lua object.
	 * \warning		Does not check whether key exists!
	 */
        luabind::object get_data(const std::string& var_name);   
	/**
	 * Retuns the set of keys.
	 * 
	 * \returns 		Vector containing all keys.
	 */
	const std::vector<std::string> get_keys();
	/**
	 * Checks whether the given key is set.
	 * 
	 * \param keys		Name of the key.
	 * \returns 		True if the mapping contains the given key.
	 */
	bool has_key(const std::string& key);
	/**
	 * Removes a key value pair.
	 * 
	 * \param key 		Name of the key.
	 */
	void remove_data(const std::string& var_name);
	/**
	 * Returns the wrapped MarkerInformation instance.
	 * 
	 * \returns		MarkerInformation instance.
	 */
        const MarkerInformation& marker_information() const;
	/**
	 * Sets the Lua state to be worked with.
	 * 
	 * Returning Luabind object requires knowing the Lua state for which
	 * the object is intended.
	 * 
	 * \param state 	The Lua state to be used.
	 */
        static void set_lua_state(boost::shared_ptr<lua_State> state);
	
    private:
        MarkerInformation marker_information_;
	static boost::shared_ptr<lua_State> lua_state_;
	
    };
}

#endif // MARKER_INFORMATION_WRAPPER_H_
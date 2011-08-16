//
//  lua_world_modifier.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 25.07.11.
//

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <luabind/object.hpp>
#include <luabind/operator.hpp>

#include "../ComputationalGeometry/misc_algorithms.h"
#include "../ComputationalGeometry/ch_algorithms.h"
#include "../ComputationalGeometry/point_algorithms.h"
#include "../ComputationalGeometry/points_sepplane.h"
#include "../Model/world_information.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Requests/position_request.h"
#include "../Utilities/console_output.h"
#include "../Utilities/vector_arithmetics.h"

#include "../Wrapper/coordinate_system_wrapper.h"
#include "../Wrapper/vector_wrapper.h"
#include "../Wrapper/marker_information_wrapper.h"
#include "../Wrapper/stats_calc_wrapper.h"
#include "../Wrapper/world_information_wrapper.h"

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
  // load cuurent WorldInformation object into wrapper
  LuaWrapper::WorldInformationWrapper::set_world_information(world_information);
     
    try {
		luabind::call_function<void>(lua_state_.get(), "main");
	}
	catch(luabind::error& e) {
		luabind::object error_msg(luabind::from_stack(e.state(), -1));
	    std::cerr << error_msg << std::endl;
	}
    
    return LuaWrapper::WorldInformationWrapper::get_request_set();  
}

void LuaWorldModifier::register_lua_methods() {
    // Lua wrappers have their own namespace
    using namespace LuaWrapper;
    // register helper classes
    luabind::module(lua_state_.get())
	[
         luabind::class_<CoordinateSystemWrapper>("CoordinateSystem")
         .def(luabind::constructor<>())
         .def(luabind::constructor<Vector3dWrapper, Vector3dWrapper, Vector3dWrapper>())
         .def_readwrite("x_axis", &CoordinateSystemWrapper::x_axis)
         .def_readwrite("y_axis", &CoordinateSystemWrapper::y_axis)
         .def_readwrite("z_axis", &CoordinateSystemWrapper::z_axis),
     
         luabind::class_<MarkerInformationWrapper>("MarkerInformation")
         .def(luabind::constructor<>())
         .def("add_data", &MarkerInformationWrapper::add_data)
         .def("get_data", &MarkerInformationWrapper::get_data)
	 .def("get_keys", &MarkerInformationWrapper::get_keys, luabind::copy_table(luabind::result))
	 .def("remove_data", &MarkerInformationWrapper::remove_data),
     
         luabind::class_<Vector3dWrapper>("Vector3d")
         .def(luabind::constructor<>())
         .def(luabind::constructor<double, double, double>())
         .def(luabind::const_self + luabind::other<Vector3dWrapper>())
         .def(luabind::const_self - luabind::other<Vector3dWrapper>())
         .def(luabind::const_self * luabind::other<Vector3dWrapper>())
         .def(luabind::const_self == luabind::other<Vector3dWrapper>())
         .def(luabind::const_self * double())
         .def(double() * luabind::const_self)
         .def(luabind::const_self / double())
         .def(luabind::tostring(luabind::self))
         .def_readwrite("x", &Vector3dWrapper::x)
         .def_readwrite("y", &Vector3dWrapper::y)
         .def_readwrite("z", &Vector3dWrapper::z)
    ];
    
    // register world information
    luabind::module(lua_state_.get())
    [
      luabind::namespace_("WorldInformation")
      [
	luabind::def("add_marker_request", &WorldInformationWrapper::add_marker_request),
	luabind::def("add_edge", (void(*)(std::size_t, std::size_t, MarkerInformationWrapper)) &WorldInformationWrapper::add_edge),
	luabind::def("add_edge", (void(*)(std::size_t, std::size_t)) &WorldInformationWrapper::add_edge),
	luabind::def("get_adjacent_edges", &WorldInformationWrapper::get_adjacent_edges, luabind::copy_table(luabind::result)),
	luabind::def("get_edge_anchors", &WorldInformationWrapper::get_edge_anchors),
	luabind::def("get_edge_information", &WorldInformationWrapper::get_edge_information),
	luabind::def("get_edges", &WorldInformationWrapper::get_edges, luabind::copy_table(luabind::result)),
	luabind::def("get_marker_information", &WorldInformationWrapper::get_marker_information),
	luabind::def("get_markers", &WorldInformationWrapper::get_markers, luabind::copy_table(luabind::result)),
	luabind::def("get_robot_information", &WorldInformationWrapper::get_robot_information),
	luabind::def("get_robots", &WorldInformationWrapper::get_robots, luabind::copy_table(luabind::result)),
	luabind::def("get_time", &WorldInformationWrapper::get_time)
      ]
    ];
}

void LuaWorldModifier::report_errors(int status) {
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(lua_state_.get(), -1) << std::endl;
		lua_pop(lua_state_.get(), 1); // remove error message
	}
}

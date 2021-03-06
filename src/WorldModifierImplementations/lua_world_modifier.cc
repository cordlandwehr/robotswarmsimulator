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

#include "../Model/world_information.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Requests/position_request.h"
#include "../Utilities/console_output.h"
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"
#include "../SimulationKernel/factories.h"

#include "../Wrapper/console_output_wrapper.h"
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
  LuaWrapper::MarkerInformationWrapper::set_lua_state(lua_state_);
     
    try {
		luabind::call_function<void>(lua_state_.get(), "main");
	}
	catch(luabind::error& e) {
		luabind::object error_msg(luabind::from_stack(e.state(), -1));
	    std::cerr << error_msg << std::endl;
	    ConsoleOutput::log(ConsoleOutput::Lua, ConsoleOutput::error) << error_msg;
	}
    
    return LuaWrapper::WorldInformationWrapper::get_request_set();  
}

void LuaWorldModifier::register_lua_methods() {
    // Lua wrappers have their own namespace
    using namespace LuaWrapper;
    // register helper functions/classes
    luabind::module(lua_state_.get())
	[
         luabind::class_<CoordinateSystemWrapper>("CoordinateSystem")
         .def(luabind::constructor<>())
         .def(luabind::constructor<Vector3dWrapper, Vector3dWrapper, Vector3dWrapper>())
         .def_readwrite("x_axis", &CoordinateSystemWrapper::x_axis)
         .def_readwrite("y_axis", &CoordinateSystemWrapper::y_axis)
         .def_readwrite("z_axis", &CoordinateSystemWrapper::z_axis),
         
         luabind::def("log", (void(*)(const std::string&)) &ConsoleOutputWrapper::log),
         luabind::def("log", (void(*)(const std::string&, const std::string&)) &ConsoleOutputWrapper::log),
     
         luabind::class_<MarkerInformationWrapper>("MarkerInformation")
         .def(luabind::constructor<>())
         .def("add_data", &MarkerInformationWrapper::add_data)
         .def("get_data", &MarkerInformationWrapper::get_data)
	 .def("get_keys", &MarkerInformationWrapper::get_keys, luabind::copy_table(luabind::result))
	 .def("has_key", &MarkerInformationWrapper::has_key)
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
	luabind::def("add_edge", (std::size_t(*)(std::size_t, std::size_t, MarkerInformationWrapper, const std::string&)) &WorldInformationWrapper::add_edge),
	luabind::def("add_edge", (std::size_t(*)(std::size_t, std::size_t, const std::string&)) &WorldInformationWrapper::add_edge),
	luabind::def("add_message", &WorldInformationWrapper::add_message),
	luabind::def("add_robot", (void(*)(std::size_t, Vector3dWrapper, std::string)) &WorldInformationWrapper::add_robot),
	luabind::def("add_robot", (void(*)(std::size_t, Vector3dWrapper, std::string, MarkerInformationWrapper)) &WorldInformationWrapper::add_robot),
	luabind::def("get_edge_information", &WorldInformationWrapper::get_edge_information),
	luabind::def("get_edges", (const std::vector<std::size_t>(*)(const std::string&)) &WorldInformationWrapper::get_edges, luabind::copy_table(luabind::result)),
	luabind::def("get_edges", (const std::vector<std::size_t>(*)(std::size_t, const std::string&)) &WorldInformationWrapper::get_edges, luabind::copy_table(luabind::result)),
	luabind::def("get_head", &WorldInformationWrapper::get_head),
	luabind::def("get_message_information", &WorldInformationWrapper::get_message_information),
	luabind::def("get_messages", (const std::vector<std::size_t>(*)()) &WorldInformationWrapper::get_messages, luabind::copy_table(luabind::result)),
	luabind::def("get_messages", (const std::vector<std::size_t>(*)(std::size_t)) &WorldInformationWrapper::get_messages, luabind::copy_table(luabind::result)),
	luabind::def("get_project_path", &WorldInformationWrapper::get_project_path),
	luabind::def("get_receiver", &WorldInformationWrapper::get_receiver),
	luabind::def("get_robot_information", &WorldInformationWrapper::get_robot_information),
	luabind::def("get_robot_position", &WorldInformationWrapper::get_robot_position),
	luabind::def("get_robots", &WorldInformationWrapper::get_robots, luabind::copy_table(luabind::result)),
	luabind::def("get_sender", &WorldInformationWrapper::get_sender),
	luabind::def("get_tail", &WorldInformationWrapper::get_tail),
	luabind::def("get_time", &WorldInformationWrapper::get_time),
	luabind::def("is_directed", &WorldInformationWrapper::is_directed),
	luabind::def("is_undirected", &WorldInformationWrapper::is_undirected),
	luabind::def("remove_edge", &WorldInformationWrapper::remove_edge),
	luabind::def("remove_message", &WorldInformationWrapper::remove_message),
	luabind::def("remove_robot", &WorldInformationWrapper::remove_robot),
	luabind::def("set_edge_information", &WorldInformationWrapper::set_edge_information),
	luabind::def("set_robot_information", &WorldInformationWrapper::set_robot_information),
	luabind::def("set_robot_position", &WorldInformationWrapper::set_robot_position)
      ]
    ];
    
    // register stats calc
    luabind::module(lua_state_.get())
    [
      luabind::namespace_("Statistics")
      [
	luabind::def("calculate_degree", &StatsCalcWrapper::calculate_degree),
	luabind::def("calculate_diameter_with_long_range", &StatsCalcWrapper::calculate_diameter_with_long_range),
	luabind::def("calculate_hop_distance", &StatsCalcWrapper::calculate_hop_distance, luabind::copy_table(_3)),
	luabind::def("calculate_maximal_defect", &StatsCalcWrapper::calculate_maximal_defect),
	luabind::def("calculate_maximal_defect", &StatsCalcWrapper::calculate_maximal_defect_default),
	luabind::def("calculate_total_defects", &StatsCalcWrapper::calculate_total_defects),
	luabind::def("evaluate_first_mf_experiment", &StatsCalcWrapper::evaluate_first_mf_experiment, luabind::copy_table(luabind::result)),
	luabind::def("evaluate_uniform_lrlink_distribution", &StatsCalcWrapper::evaluate_uniform_lrlink_distribution, luabind::copy_table(luabind::result)),
	luabind::def("evaluate_polynomial", &StatsCalcWrapper::evaluate_polynomial, luabind::copy_table(_1)),
	luabind::def("generate_primes", &StatsCalcWrapper::generate_primes, luabind::copy_table(luabind::result))
	
      ]
    ];
	
	//register distribution generator		 
    luabind::module(lua_state_.get())
    [	
    
	  luabind::class_<DistributionGenerator>("DistributionGenerator")
			 .def(luabind::constructor<int>())
			 .def("set_seed", &DistributionGenerator::set_seed)
			 .def("init_uniform", &DistributionGenerator::init_uniform)
			 .def("init_normal", &DistributionGenerator::init_normal)
			 .def("init_bernoulli", &DistributionGenerator::init_bernoulli)
			 .def("init_exponential", &DistributionGenerator::init_exponential)
			 .def("init_geometric", &DistributionGenerator::init_geometric)
			 .def("init_uniform_real", &DistributionGenerator::init_uniform_real)
			 .def("init_uniform_on_sphere", &DistributionGenerator::init_uniform_on_sphere)
			 .def("get_value_uniform", &DistributionGenerator::get_value_uniform)
			 .def("get_value_normal", &DistributionGenerator::get_value_normal)
			 .def("get_value_bernoulli", &DistributionGenerator::get_value_bernoulli)
			 .def("get_value_exponential", &DistributionGenerator::get_value_exponential)
			 .def("get_value_geometric", &DistributionGenerator::get_value_geometric)
			 .def("get_value_uniform_real", &DistributionGenerator::get_value_uniform_real)
			 .def("get_value_uniform_on_sphere", &DistributionGenerator::get_value_uniform_on_sphere, luabind::copy_table(luabind::result))
			 
	];
	
    
}

void LuaWorldModifier::report_errors(int status) {
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(lua_state_.get(), -1) << std::endl;
		lua_pop(lua_state_.get(), 1); // remove error message
	}
}


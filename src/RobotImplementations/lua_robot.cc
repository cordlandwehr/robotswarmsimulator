/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * lua_robot.cc
 *
 *  Created on: 07.02.2009
 *      Author: Daniel
 */

#include "lua_robot.h"

#include <algorithm>
#include <deque>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string.hpp>


#include <luabind/object.hpp>
#include <luabind/operator.hpp>

#include "../Model/robot_identifier.h"
#include "../Model/marker_identifier.h"
#include "../Model/edge_identifier.h"
#include "../Model/message_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/box_identifier.h"
#include "../Model/sphere_identifier.h"
#include "../Model/marker_information.h"
#include "../Requests/marker_request.h"
#include "../Requests/insert_edge_request.h"
#include "../Requests/remove_edge_request.h"
#include "../Requests/remove_message_request.h"
#include "../Requests/send_message_request.h"
#include "../Requests/position_request.h"
#include "../Views/view.h"
#include "../Wrapper/lua_distribution_generator.h"
#include "../Wrapper/coordinate_system_wrapper.h"
#include "../Wrapper/vector_wrapper.h"
#include "../Wrapper/marker_information_wrapper.h"
#include "../Wrapper/console_output_wrapper.h"
#include "../Wrapper/stats_calc_wrapper.h"

namespace {
	boost::shared_ptr<View> view; //current view for the lua script
	boost::shared_ptr<lua_State> lua;
	Robot* robot; //needed as "caller" in most view methods
	
	std::map< std::size_t, boost::shared_ptr<EdgeIdentifier> > edge_identifiers_;
	std::map< std::size_t, boost::shared_ptr<MessageIdentifier> > message_identifiers_;
	std::map< std::size_t, boost::shared_ptr<RobotIdentifier> > robot_identifiers_;
	
	std::set<boost::shared_ptr<Request> > requests;
	
   /**
   * Checks reference map.
   * 
   * Checks for a given pair of a reference map and a key (ID), whether the 
   * key exists. Throws an exception if the key does not exist.
   * 
   * \param map		Map to be checked.
   * \param key		Key (ID) to be looked up.
   */
	template<typename T> void 
	check_mapping(const std::map<std::size_t, boost::shared_ptr<T> >& map, std::size_t key) {
	  // get iterator for given key
	  typename std::map<std::size_t, boost::shared_ptr<T> >::const_iterator it;
	  it = map.find(key);
	  // check entry
	  if (it == map.end()) {
	    throw std::invalid_argument("The given ID is unknown to the Lua Wrapper. Possible reasons: The ID does not exists. The ID has not been looked up before.");
	  }
	}
	
	const std::vector<std::size_t> get_ids(const std::vector< boost::shared_ptr<EdgeIdentifier> > ids) {
	  std::vector<std::size_t> result;
	  BOOST_FOREACH(boost::shared_ptr<EdgeIdentifier> ident, ids) {
	    std::size_t id = view->get_id(*robot, ident);
	    edge_identifiers_[id] = ident;
	    // update robots ...
	    boost::shared_ptr<RobotIdentifier> tail = view->get_edge_source(ident);
	    boost::shared_ptr<RobotIdentifier> head = view->get_edge_target(ident);
	    robot_identifiers_[view->get_id(*robot, tail)] = tail;
	    robot_identifiers_[view->get_id(*robot, head)] = head;
	    // store ID
	    result.push_back(id);
	  }
	  return result;
	}
	
	const std::size_t get_id(const boost::shared_ptr<MessageIdentifier> ident) {
	  std::size_t id = view->get_id(*robot, ident);
	  message_identifiers_[id] = ident;
	  // update sender robot (if visible)
	  /*boost::shared_ptr<RobotIdentifier> sender = view->get_sender(ident);
	  if (sender) {
	    robot_identifiers_[view->get_id(*robot, sender)] = sender;
	  }*/
	  // return integer ID
	  return id;
	}
		
	const std::vector<std::size_t> get_ids(const std::vector< boost::shared_ptr<RobotIdentifier> > ids) {
	  std::vector<std::size_t> result;
	  BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> ident, ids) {
	    std::size_t id = view->get_id(*robot, ident);
	    robot_identifiers_[id] = ident;
	    result.push_back(id);
	  }
	  return result;
	}

	/**
	 * @see View.get_visible_robots()
	 * @return Array of identifiers
	 */
	const std::vector<std::size_t> get_visible_robots() {
	    return get_ids(view->get_visible_robots(*robot));
	}
	
	const std::vector<std::size_t> get_visible_edges() {
	    return get_ids(view->get_visible_edges(*robot));
	}
	
	const std::size_t get_number_of_messages() {
	  return view->get_number_of_messages(*robot);
	}
	
	const std::size_t get_message(std::size_t index) {
	  return get_id(view->get_message(*robot, index));
	}
	
	/**
	 * @param Identifier
	 * @return MarkerInformation of the object identified by the given identifier.
	 * @see View.get_marker_information()
	 */

	const LuaWrapper::MarkerInformationWrapper get_marker_information(boost::shared_ptr<Identifier> id) {
		return LuaWrapper::MarkerInformationWrapper(view->get_marker_information(*robot, id));
	}
	
	const LuaWrapper::MarkerInformationWrapper get_edge_information(std::size_t id) {
	  // check the given ID
	  check_mapping(edge_identifiers_, id);
	  // delegate call to view
	  return get_marker_information(edge_identifiers_[id]);
	}

	const LuaWrapper::MarkerInformationWrapper get_message_information(std::size_t id) {
	  // check the given ID
	  check_mapping(message_identifiers_, id);
	  // delegate call to view
	  return get_marker_information(message_identifiers_[id]);
	}
	
	const LuaWrapper::MarkerInformationWrapper get_robot_information(std::size_t id) {
	  // check the given ID
	  check_mapping(robot_identifiers_, id);
	  // delegate call to view
	  return get_marker_information(robot_identifiers_[id]);
	}
		
	/**
	 * @param (Robot-)Identifier
	 * @return Returns whether last (already performed) request has been successful (i.e. was handled in exactly the way
	 *         the issuer requested) for the robot with the given identifier.
	 * @see View.get_robot_last_request_successful()
	 */
	
	const bool get_robot_last_request_successful(std::size_t id) {
	  // check the given id
	  check_mapping(robot_identifiers_, id);
	  // delagte to view
	  return view->get_robot_last_request_successful(*robot, robot_identifiers_[id]);
	}
	
	
	std::size_t get_tail(std::size_t id) {
	  // check the given ID
	  check_mapping(edge_identifiers_, id);
	  // return robot ID
	  return view->get_id(*robot, view->get_edge_source(edge_identifiers_[id]));
	}
	
	std::size_t get_head(std::size_t id) {
	 // check the given ID
	 check_mapping(edge_identifiers_, id);
	 // return robot ID
	 return view->get_id(*robot, view->get_edge_target(edge_identifiers_[id]));
	}
	
	luabind::object get_sender(std::size_t id) {
	  // check the given ID
	  check_mapping(message_identifiers_, id);
	  // get and check Robotidentifier
	  boost::shared_ptr<RobotIdentifier> sender = view->get_sender(message_identifiers_[id]);
	  if (sender) {
	    // view returned an existing ID (the sender is visible), return integer ID
	    std::size_t sender_id = view->get_id(*robot, sender);
	    return luabind::object(lua.get(), sender_id);
	  } else {
	    // view returned NULL pointer (the seder is invisible), return NIL
	    return luabind::object();
	  }
	}
	
	/**
	 * @return time of view
	 * @see View.get_time()
	 */

	const int get_time() {
		return view->get_time();
	}

	bool is_directed(std::size_t id) {
	  // check the given ID
	  check_mapping(edge_identifiers_, id);
	  // look up edge adn call view
	  return view->is_edge_directed(edge_identifiers_[id]);
	}

	bool is_undirected(std::size_t id) {
	  return !is_directed(id);
	}

	void add_insert_edge_request(std::size_t tail, std::size_t head, LuaWrapper::MarkerInformationWrapper marker, const std::string& type) {
	    // get robot IDs
	    boost::shared_ptr<RobotIdentifier> source_robot(new RobotIdentifier(tail));
	    boost::shared_ptr<RobotIdentifier> target_robot(new RobotIdentifier(head));
	    // create new edge (decide type depending on given string)
	    std::string type_lower(type);
	    boost::to_lower(type_lower);
	    boost::shared_ptr<Edge> edge;
	    if (type_lower == "directed") {
	      edge.reset(new DirectedEdge(source_robot, target_robot));
	    } else {
	      edge.reset(new UndirectedEdge(source_robot, target_robot));
	    }
	    // create and assign new MarkerInforamtion object
	    boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
	    edge->set_marker_information(new_marker);
	    // create request
	    requests.insert(boost::shared_ptr<Request>(new InsertEdgeRequest(*robot, edge)));
	}
	
	void add_insert_edge_request(std::size_t tail, std::size_t head, const std::string& type) {
	  LuaWrapper::MarkerInformationWrapper marker;
	  add_insert_edge_request(tail, head, marker, type);
	}
	
	void add_remove_edge_request(std::size_t id) {
	  // check the given ID
	  check_mapping(edge_identifiers_, id);
	  // create request
	  requests.insert(boost::shared_ptr<Request>(new RemoveEdgeRequest(*robot, edge_identifiers_[id])));
	}
	
	void add_send_message_request(std::size_t id, LuaWrapper::MarkerInformationWrapper marker) {
	  // check the given ID
	  check_mapping(robot_identifiers_, id);
	  // cerate new Message
	  boost::shared_ptr<Message> message(new Message(robot->id(), robot_identifiers_[id]));
	  boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
	  message->set_marker_information(new_marker);
	  // create request
	  requests.insert(boost::shared_ptr<Request>(new SendMessageRequest(*robot, message)));
	}
	
	void add_remove_message_request(std::size_t id) {
	  // check the given ID
	  check_mapping(message_identifiers_, id);
	  // create request
	  requests.insert(boost::shared_ptr<Request>(new RemoveMessageRequest(*robot, message_identifiers_[id])));
	}
	
	/**
	 * Adds a PositionRequest which is send to the simulation as return value of
	 * the compute method.
	 * @param Requested position vector
	 */

	void add_position_request(LuaWrapper::Vector3dWrapper requested_vector) {
		boost::shared_ptr<Vector3d> new_pos(new Vector3d(transform(requested_vector)));
		requests.insert(boost::shared_ptr<Request>(new PositionRequest(*robot, new_pos)));
	}

	/**
	 * Adds a MarkerRequest which is send to the simulation as return value of
	 * the compute method.
	 * @param Requested MarkerInformation
	 */

	void add_marker_request(LuaWrapper::MarkerInformationWrapper marker) {
		boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(marker.marker_information()));
		requests.insert(boost::shared_ptr<Request>(new MarkerRequest(*robot, new_marker)));
	}

	/**
	 * Returns the identifier of this robot.
	 * @return Identifier
	 */

	const unsigned get_own_identifier() {
		std::size_t id = view->get_id(*robot, robot->id());
		robot_identifiers_[id] = robot->id();
		return id;
	}

}

void LuaRobot::report_errors(int status) {
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(lua_state_.get(), -1) << std::endl;
		lua_pop(lua_state_.get(), 1); // remove error message
	}
}

LuaRobot::~LuaRobot() {

}

LuaRobot::LuaRobot(boost::shared_ptr<RobotIdentifier> id, const std::string& lua_file_name)
                   : Robot(id), lua_file_name_(lua_file_name), lua_state_(lua_open(), lua_close) {
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

void LuaRobot::register_lua_methods() {
	//register view methods to lua
	luabind::module(lua_state_.get())
	[
		 luabind::class_<LuaWrapper::Vector3dWrapper>("Vector3d")
			 .def(luabind::constructor<>())
			 .def(luabind::constructor<double, double, double>())
			 .def(luabind::const_self + luabind::other<LuaWrapper::Vector3dWrapper>())
			 .def(luabind::const_self - luabind::other<LuaWrapper::Vector3dWrapper>())
			 .def(luabind::const_self * luabind::other<LuaWrapper::Vector3dWrapper>())
			 .def(luabind::const_self == luabind::other<LuaWrapper::Vector3dWrapper>())
			 .def(luabind::const_self * double())
			 .def(double() * luabind::const_self)
			 .def(luabind::const_self / double())
			 .def(luabind::tostring(luabind::self))
			 .def_readwrite("x", &LuaWrapper::Vector3dWrapper::x)
			 .def_readwrite("y", &LuaWrapper::Vector3dWrapper::y)
			 .def_readwrite("z", &LuaWrapper::Vector3dWrapper::z),

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
			 ,

		 luabind::class_<LuaWrapper::MarkerInformationWrapper>("MarkerInformation")
		      .def(luabind::constructor<>())
		      .def("add_data", &LuaWrapper::MarkerInformationWrapper::add_data)
		      .def("get_data", &LuaWrapper::MarkerInformationWrapper::get_data)
		      .def("get_keys", &LuaWrapper::MarkerInformationWrapper::get_keys, luabind::copy_table(luabind::result))
		      .def("has_key", &LuaWrapper::MarkerInformationWrapper::has_key)
		      .def("remove_data", &LuaWrapper::MarkerInformationWrapper::remove_data),
		   
		  luabind::def("log", (void(*)(const std::string&)) &ConsoleOutputWrapper::log),
		  luabind::def("log", (void(*)(const std::string&, const std::string&)) &ConsoleOutputWrapper::log),
		   
		// now our view-functions
		// TODO (cola) still commented out, cause this will cause trouble on the next upstream ;)
		luabind::namespace_("View")
		[
			 luabind::def("add_insert_edge_request", (void(*)(std::size_t, std::size_t, LuaWrapper::MarkerInformationWrapper, const std::string&)) &add_insert_edge_request),
			 luabind::def("add_insert_edge_request", (void(*)(std::size_t, std::size_t, const std::string&)) &add_insert_edge_request),
			 luabind::def("add_position_request", &add_position_request),
			 luabind::def("add_marker_request", &add_marker_request),
			 luabind::def("add_send_message_request", &add_send_message_request),
			 luabind::def("add_remove_edge_request", &add_remove_edge_request),
			 luabind::def("add_remove_message_request", &add_remove_message_request),
			 luabind::def("get_edge_information", &get_edge_information),
			 luabind::def("get_head", &get_head),
			 luabind::def("get_visible_edges", &get_visible_edges, luabind::copy_table(luabind::result)), 
			 luabind::def("get_message", &get_message),
			 luabind::def("get_message_information", &get_message_information),
			 luabind::def("get_number_of_messages", &get_number_of_messages),
			 luabind::def("get_own_id", &get_own_identifier),
			 luabind::def("get_robot_information", &get_robot_information),
			 luabind::def("get_robot_last_request_successful", &get_robot_last_request_successful),
			 luabind::def("get_sender", &get_sender),
			 luabind::def("get_tail", &get_tail),
			 luabind::def("get_time", &get_time),
			 luabind::def("get_visible_robots", &get_visible_robots, luabind::copy_table(luabind::result)),
			 luabind::def("is_directed", &is_directed),
			 luabind::def("is_undirected", &is_undirected)
	    ],

	    luabind::namespace_("Statistics")
		[
		  luabind::def("evaluate_polynomial", &LuaWrapper::StatsCalcWrapper::evaluate_polynomial, luabind::copy_table(_1))
		]
	];

}

std::set<boost::shared_ptr<Request> > LuaRobot::compute() {
	view = view_;
	lua = lua_state_;
	robot = this;
	
	LuaWrapper::MarkerInformationWrapper::set_lua_state(lua_state_);
	
	edge_identifiers_.clear();
	message_identifiers_.clear();
	robot_identifiers_.clear();
	
	requests.clear();
	
	// TODO: Get rid of this workaround ...
	get_visible_robots();

	try {
		luabind::call_function<void>(lua_state_.get(), "main");
	}
	catch(luabind::error& e) {
		luabind::object error_msg(luabind::from_stack(e.state(), -1));
	    std::cerr << error_msg << std::endl;
	}

	return requests;
}

std::string LuaRobot::get_algorithm_id () const {
	return lua_file_name_;
}




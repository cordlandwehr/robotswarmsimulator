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


#include <luabind/object.hpp>
#include <luabind/operator.hpp>

#include "../Model/robot_identifier.h"
#include "../Model/marker_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/box_identifier.h"
#include "../Model/sphere_identifier.h"
#include "../Model/marker_information.h"
#include "../Requests/marker_request.h"
#include "../Requests/position_request.h"
#include "../Views/view.h"
#include "../Wrapper/lua_distribution_generator.h"
#include "../Wrapper/coordinate_system_wrapper.h"
#include "../Wrapper/vector_wrapper.h"
#include "../Wrapper/marker_information_wrapper.h"

namespace {
	boost::shared_ptr<View> view; //current view for the lua script
	Robot* robot; //needed as "caller" in most view methods
	std::deque<boost::shared_ptr<Identifier> > queried_identifiers;
	std::set<boost::shared_ptr<Request> > requests;

	/**
	 * Wrapper for RobotType. Allows lua scripts to work with RobotType constants (access using e.g. RobotType.MASTER).
	 * @see RobotType
	 */

	struct RobotTypeWrapper {

	};

	/**
	 * Wrapper for RobotStatus. Allows lua scripts to work with RobotType constants (access using e.g. RobotStatus.READY).
	 * @see RobotStatus
	 */

	struct RobotStatusWrapper {

	};

	template<typename T>
	const boost::shared_ptr<T> resolve(std::size_t identifier_index) {
		boost::shared_ptr<T> result = boost::dynamic_pointer_cast<T>(queried_identifiers[identifier_index]);
		if(!result) {
			throw std::invalid_argument("Invalid type: Index " + boost::lexical_cast<std::string>(identifier_index) + " does not point to " + typeid(T).name() + ".");
		}
		return result;
	}

	template<typename T>
	const std::vector<std::size_t> transform(const std::vector<boost::shared_ptr<T> >& set) {
		std::size_t begin_index = queried_identifiers.size();
		queried_identifiers.insert(queried_identifiers.end(), set.begin(), set.end());
		std::vector<std::size_t> result(set.size());
		std::for_each(result.begin(), result.end(), boost::lambda::_1 = boost::lambda::var(begin_index)++);
		return result;
	}

	const LuaWrapper::MarkerInformationWrapper transform(const MarkerInformation& marker) {
		return LuaWrapper::MarkerInformationWrapper(marker);
	}

	const LuaWrapper::CoordinateSystemWrapper transform(const boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> >& cs) {
		return LuaWrapper::CoordinateSystemWrapper(LuaWrapper::transform(*boost::get<0>(cs)), LuaWrapper::transform(*boost::get<1>(cs)), LuaWrapper::transform(*boost::get<2>(cs)));
	}

	const MarkerInformation transform(const LuaWrapper::MarkerInformationWrapper& marker) {
		return marker.marker_information();
	}

	/**
	 * @see View.get_visible_robots()
	 * @return Array of identifiers
	 */

	const std::vector<std::size_t> get_visible_robots() {
		return transform(view->get_visible_robots(*robot));
	}

	/**
	 * @see View.get_visible_obstacles()
	 * @return Array of identifiers
	 */

	const std::vector<std::size_t> get_visible_obstacles() {
		return transform(view->get_visible_obstacles(*robot));
	}


	/**
	 * @see View.get_visible_markers()
	 * @return Array of identifiers
	 */
	const std::vector<std::size_t> get_visible_markers() {
		return transform(view->get_visible_markers(*robot));
	}

	/**
	 * @param Identifier
	 * @return Position of the object identified by the given identifier.
	 * @see View.get_position()
	 */

	const LuaWrapper::Vector3dWrapper get_position(std::size_t index) {
		return LuaWrapper::transform(view->get_position(*robot, resolve<Identifier>(index)));
	}

	/**
	 * @param Identifier
	 * @return MarkerInformation of the object identified by the given identifier.
	 * @see View.get_marker_information()
	 */

	const LuaWrapper::MarkerInformationWrapper get_marker_information(std::size_t index) {
		return transform(view->get_marker_information(*robot, resolve<Identifier>(index)));
	}

	/**
	 * @param Identifier
	 * @return Id of the object identified by the given identifier.
	 * @see View.get_id()
	 */

	const std::size_t get_id(std::size_t index) {
		return view->get_id(*robot, resolve<Identifier>(index));
	}

	/**
	 * @param (Robot-)Identifier
	 * @return Acceleration of the robot identified by the given identifier.
	 * @see View.get_robot_acceleration()
	 */

	const LuaWrapper::Vector3dWrapper get_robot_acceleration(std::size_t index) {
		return LuaWrapper::transform(view->get_robot_acceleration(*robot, resolve<RobotIdentifier>(index)));
	}

	/**
	 * @param (Robot-)Identifier
	 * @return Coordinate system origin of the robot identified by the given identifier.
	 * @see View.get_robot_coordinate_system_origin()
	 */

	const LuaWrapper::Vector3dWrapper get_robot_coordinate_system_origin(std::size_t index) {
		return LuaWrapper::transform(view->get_robot_coordinate_system_origin(*robot, resolve<RobotIdentifier>(index)));
	}

	/**
	 * @param (Robot-)Identifier
	 * @return CoordinateSystem of the robot identified by the given identifier.
	 * @see View.get_robot_coordinate_system_axis()
	 */

	const LuaWrapper::CoordinateSystemWrapper get_robot_coordinate_system_axis(std::size_t index) {
		return transform(view->get_robot_coordinate_system_axis(*robot, resolve<RobotIdentifier>(index)));
	}

	/**
	 * @param (Robot-)Identifier
	 * @return RobotType of the robot identified by the given identifier.
	 * @see View.get_robot_type()
	 */

	const unsigned get_robot_type(std::size_t index) {
		return view->get_robot_type(*robot, resolve<RobotIdentifier>(index));
	}

	/**
	 * @param (Robot-)Identifier
	 * @return RobotStatus of the robot identified by the given identifier.
	 * @see View.get_robot_status()
	 */

	const unsigned get_robot_status(std::size_t index) {
		return view->get_robot_status(*robot, resolve<RobotIdentifier>(index));
	}

	/**
	 * @param (Robot-)Identifier
	 * @return Returns whether last (already performed) request has been successful (i.e. was handled in exactly the way
	 *         the issuer requested) for the robot with the given identifier.
	 * @see View.get_robot_last_request_successful()
	 */

	const unsigned get_robot_last_request_successful(std::size_t index) {
		return view->get_robot_last_request_successful(*robot, resolve<RobotIdentifier>(index));
	}

	/**
	 * @param (Obstacle-)Identifier
	 * @return true if point is in given obstacle; false otherwise
	 * @see View.is_point_in_obstacle()
	 */

	const bool is_point_in_obstacle(std::size_t index, LuaWrapper::Vector3dWrapper point) {
		return view->is_point_in_obstacle(resolve<ObstacleIdentifier>(index), LuaWrapper::transform(point));
	}

	/**
	 * @param (Box-)Identifier
	 * @return Depth of the Box identified by the given identifier.
	 * @see View.get_box_depth()
	 */

	const double get_box_depth(std::size_t index) {
		return view->get_box_depth(resolve<BoxIdentifier>(index));
	}

	/**
	 * @param (Box-)Identifier
	 * @return Width of the Box identified by the given identifier.
	 * @see View.get_box_width()
	 */

	const double get_box_width(std::size_t index) {
		return view->get_box_width(resolve<BoxIdentifier>(index));
	}

	/**
	 * @param (Box-)Identifier
	 * @return Height of the Box identified by the given identifier.
	 * @see View.get_box_height()
	 */

	const double get_box_height(std::size_t index) {
		return view->get_box_height(resolve<BoxIdentifier>(index));
	}

	/**
	 * @param (Sphere-)Identifier
	 * @return Radius of the Sphere identified by the given identifier.
	 * @see View.get_sphere_radius()
	 */

	const double get_sphere_radius(std::size_t index) {
		return view->get_sphere_radius(resolve<SphereIdentifier>(index));
	}

	/**
	 * @return time of view
	 * @see View.get_time()
	 */

	const int get_time() {
		return view->get_time();
	}

	/**
	 * Checks if the given Identifier is a SphereIdentifier
	 * @param Identifier
	 * @return true if given Identifier is a SphereIdentifier; false otherwise
	 */

	const bool is_sphere_identifier(std::size_t index) {
		return boost::dynamic_pointer_cast<SphereIdentifier>(resolve<Identifier>(index));
	}

	/**
	 * Checks if the given Identifier is a BoxIdentifier
	 * @param Identifier
	 * @return true if given Identifier is a BoxIdentifier; false otherwise
	 */

	const bool is_box_identifier(std::size_t index) {
		return boost::dynamic_pointer_cast<BoxIdentifier>(resolve<Identifier>(index));
	}

	/**
	 * Adds a AccelerationRequest which is send to the simulation as return value of
	 * the compute method.
	 * @param Requested acceleration vector
	 */

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
		boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(transform(marker)));
		requests.insert(boost::shared_ptr<Request>(new MarkerRequest(*robot, new_marker)));
	}

	/**
	 * Returns the identifier of this robot.
	 * @return Identifier
	 */

	const unsigned get_own_identifier() {
		return 0; //own id always at pos 0 in queried_identifiers
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
			 .def("get_data", &LuaWrapper::MarkerInformationWrapper::get_data),

		 luabind::class_<LuaWrapper::CoordinateSystemWrapper>("CoordinateSystem")
			 .def(luabind::constructor<>())
			 .def(luabind::constructor<LuaWrapper::Vector3dWrapper, LuaWrapper::Vector3dWrapper, LuaWrapper::Vector3dWrapper>())
			 .def_readwrite("x_axis", &LuaWrapper::CoordinateSystemWrapper::x_axis)
			 .def_readwrite("y_axis", &LuaWrapper::CoordinateSystemWrapper::y_axis)
			 .def_readwrite("z_axis", &LuaWrapper::CoordinateSystemWrapper::z_axis),

		 luabind::class_<RobotTypeWrapper>("RobotType")
			 .enum_("constants")
			 [
				  luabind::value("MASTER", MASTER),
				  luabind::value("SLAVE", SLAVE)
			 ],

 		 luabind::class_<RobotStatusWrapper>("RobotStatus")
			 .enum_("constants")
			 [
			 	  luabind::value("SLEEPING", SLEEPING),
			 	  luabind::value("READY", READY)
			 ],

		// now our view-functions
		// TODO (cola) still commented out, cause this will cause trouble on the next upstream ;)
		luabind::namespace_("View")
		[
			 luabind::def("get_visible_robots", &get_visible_robots, luabind::copy_table(luabind::result)),
			 luabind::def("get_visible_obstacles", &get_visible_obstacles, luabind::copy_table(luabind::result)),
			 luabind::def("get_visible_markers", &get_visible_markers, luabind::copy_table(luabind::result)),
			 luabind::def("get_position", &get_position),
			 luabind::def("get_marker_information", &get_marker_information),
			 luabind::def("get_id", &get_id),
			 luabind::def("get_robot_acceleration", &get_robot_acceleration),
			 luabind::def("get_robot_coordinate_system_origin", &get_robot_coordinate_system_origin),
			 luabind::def("get_robot_coordinate_system_axis", &get_robot_coordinate_system_axis),
			 luabind::def("get_robot_type", &get_robot_type),
			 luabind::def("get_robot_status", &get_robot_status),
			 luabind::def("get_robot_last_request_successful", &get_robot_last_request_successful),
			 luabind::def("is_point_in_obstacle", &is_point_in_obstacle),
			 luabind::def("get_box_depth", &get_box_depth),
			 luabind::def("get_box_width", &get_box_width),
			 luabind::def("get_box_height", &get_box_height),
			 luabind::def("get_sphere_radius", &get_sphere_radius),
			 luabind::def("get_time", &get_time),
			 luabind::def("is_sphere_identifier", &is_sphere_identifier),
			 luabind::def("is_box_identifier", &is_box_identifier),
			 luabind::def("add_position_request", &add_position_request),
			 luabind::def("add_marker_request", &add_marker_request),
			 luabind::def("get_own_identifier", &get_own_identifier)
	    ],

		luabind::def("get_visible_robots", &get_visible_robots, luabind::copy_table(luabind::result)),
		luabind::def("get_visible_obstacles", &get_visible_obstacles, luabind::copy_table(luabind::result)),
		luabind::def("get_visible_markers", &get_visible_markers, luabind::copy_table(luabind::result)),
		luabind::def("get_position", &get_position),
		luabind::def("get_marker_information", &get_marker_information),
		luabind::def("get_id", &get_id),
		luabind::def("get_robot_acceleration", &get_robot_acceleration),
		luabind::def("get_robot_coordinate_system_origin", &get_robot_coordinate_system_origin),
		luabind::def("get_robot_coordinate_system_axis", &get_robot_coordinate_system_axis),
		luabind::def("get_robot_type", &get_robot_type),
		luabind::def("get_robot_status", &get_robot_status),
		luabind::def("get_robot_last_request_successful", &get_robot_last_request_successful),
		luabind::def("is_point_in_obstacle", &is_point_in_obstacle),
		luabind::def("get_box_depth", &get_box_depth),
		luabind::def("get_box_width", &get_box_width),
		luabind::def("get_box_height", &get_box_height),
		luabind::def("get_sphere_radius", &get_sphere_radius),
		luabind::def("get_time", &get_time),
		luabind::def("is_sphere_identifier", &is_sphere_identifier),
		luabind::def("is_box_identifier", &is_box_identifier),
		luabind::def("add_position_request", &add_position_request),
		luabind::def("add_marker_request", &add_marker_request),
		luabind::def("get_own_identifier", &get_own_identifier),

		// some functions to access global number generator from lua.
		luabind::def("gen_init_uniform", &LuaWrapper::lua_generator_init_uniform),
		luabind::def("gen_get_uniform", &LuaWrapper::lua_generator_get_uniform)

	];

}

std::set<boost::shared_ptr<Request> > LuaRobot::compute() {
	view = view_;
	robot = this;
	requests.clear();
	queried_identifiers.clear();
	queried_identifiers.push_back(id());

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




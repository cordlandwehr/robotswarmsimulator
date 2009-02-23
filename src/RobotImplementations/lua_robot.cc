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
#include "../Requests/acceleration_request.h"
#include "../Requests/marker_request.h"
#include "../Requests/position_request.h"
#include "../Requests/type_change_request.h"
#include "../Requests/velocity_request.h"
#include "../Views/view.h"

namespace {
	boost::shared_ptr<View> view; //current view for the lua script
	Robot* robot; //needed as "caller" in most view methods
	std::deque<boost::shared_ptr<Identifier> > queried_identifiers;
	std::set<boost::shared_ptr<Request> > requests;

	struct Vector3dWrapper {
		Vector3dWrapper() : x(0), y(0), z(0) {;}
		Vector3dWrapper(double x, double y, double z) : x(x), y(y), z(z) {;}

		const Vector3dWrapper operator+(const Vector3dWrapper& rhs) const {
			return Vector3dWrapper(x + rhs.x, y + rhs.y, z + rhs.z);
		}

		const Vector3dWrapper operator*(const Vector3dWrapper& rhs) const {
			return Vector3dWrapper(x * rhs.x, y * rhs.y, z * rhs.z);
		}

		const Vector3dWrapper operator/(double div) const {
			return Vector3dWrapper(x / div, y / div, z / div);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector3dWrapper& rhs) {
			//Note: not implemented using operator<< of Vector3d, because latter is ugly.
			return os << "{x = " << rhs.x << ", y = " << rhs.y << ", z = " << rhs.z << "}";
		}


		double x;
		double y;
		double z;

	};

	class MarkerInformationWrapper {
	public:
		MarkerInformationWrapper() : marker_information_() {;}
		MarkerInformationWrapper(const MarkerInformation& marker_information) : marker_information_(marker_information) {;}

		void add_data(const std::string& var_name, const luabind::object& object) {
			marker_information_.add_data(var_name, object);
		}

		luabind::object get_data(const std::string& var_name) {
			return boost::any_cast<luabind::object>(marker_information_.get_data(var_name));
		}

		const MarkerInformation& marker_information() const {
			return marker_information_;
		}
	private:
		MarkerInformation marker_information_;
	};

	struct CoordinateSystemWrapper {
		CoordinateSystemWrapper() {;}
		CoordinateSystemWrapper(Vector3dWrapper x_axis, Vector3dWrapper y_axis, Vector3dWrapper z_axis) : x_axis(x_axis), y_axis(y_axis), z_axis(z_axis) {;}
		Vector3dWrapper x_axis;
		Vector3dWrapper y_axis;
		Vector3dWrapper z_axis;
	};

	struct RobotTypeWrapper {

	};

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
	const std::vector<std::size_t> transform(const std::set<boost::shared_ptr<T> >& set) {
		std::size_t begin_index = queried_identifiers.size();
		queried_identifiers.insert(queried_identifiers.end(), set.begin(), set.end());
		std::vector<std::size_t> result(set.size());
		std::for_each(result.begin(), result.end(), boost::lambda::_1 = boost::lambda::var(begin_index)++);
		return result;
	}

	const Vector3dWrapper transform(const Vector3d& vec) {
		return Vector3dWrapper(vec(0), vec(1), vec(2));
	}

	const MarkerInformationWrapper transform(const MarkerInformation& marker) {
		return MarkerInformationWrapper(marker);
	}

	const CoordinateSystemWrapper transform(const boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> >& cs) {
		return CoordinateSystemWrapper(transform(*boost::get<0>(cs)), transform(*boost::get<1>(cs)), transform(*boost::get<2>(cs)));
	}

	const Vector3d transform(const Vector3dWrapper& vec) {
		Vector3d result;
		result.insert_element(kXCoord, vec.x);
		result.insert_element(kYCoord, vec.y);
		result.insert_element(kZCoord, vec.z);
		return result;
	}

	const MarkerInformation transform(const MarkerInformationWrapper& marker) {
		return marker.marker_information();
	}

	const std::vector<std::size_t> get_visible_robots() {
		return transform(view->get_visible_robots(*robot));
	}

	const std::vector<std::size_t> get_visible_obstacles() {
		return transform(view->get_visible_obstacles(*robot));
	}

	const std::vector<std::size_t> get_visible_markers() {
		return transform(view->get_visible_markers(*robot));
	}

	const Vector3dWrapper get_position(std::size_t index) {
		return transform(view->get_position(*robot, resolve<Identifier>(index)));
	}

	const MarkerInformationWrapper get_marker_information(std::size_t index) {
		return transform(view->get_marker_information(*robot, resolve<Identifier>(index)));
	}

	const std::size_t get_id(std::size_t index) {
		return view->get_id(*robot, resolve<Identifier>(index));
	}

	const Vector3dWrapper get_robot_acceleration(std::size_t index) {
		return transform(view->get_robot_acceleration(*robot, resolve<RobotIdentifier>(index)));
	}

	const CoordinateSystemWrapper get_robot_coordinate_system_axis(std::size_t index) {
		return transform(view->get_robot_coordinate_system_axis(*robot, resolve<RobotIdentifier>(index)));
	}

	const unsigned get_robot_type(std::size_t index) {
		return view->get_robot_type(*robot, resolve<RobotIdentifier>(index));
	}

	const unsigned get_robot_status(std::size_t index) {
		return view->get_robot_status(*robot, resolve<RobotIdentifier>(index));
	}

	const bool is_point_in_obstacle(std::size_t index, Vector3dWrapper point) {
		return view->is_point_in_obstacle(resolve<ObstacleIdentifier>(index), transform(point));
	}

	const double get_box_depth(std::size_t index) {
		return view->get_box_depth(resolve<BoxIdentifier>(index));
	}

	const double get_box_width(std::size_t index) {
		return view->get_box_width(resolve<BoxIdentifier>(index));
	}

	const double get_box_height(std::size_t index) {
		return view->get_box_height(resolve<BoxIdentifier>(index));
	}

	const double get_sphere_radius(std::size_t index) {
		return view->get_sphere_radius(resolve<SphereIdentifier>(index));
	}

	const bool is_sphere_identifier(std::size_t index) {
		return boost::dynamic_pointer_cast<SphereIdentifier>(resolve<Identifier>(index));
	}

	const bool is_box_identifier(std::size_t index) {
		return boost::dynamic_pointer_cast<BoxIdentifier>(resolve<Identifier>(index));
	}

	void add_acceleration_request(Vector3dWrapper requested_vector) {
		boost::shared_ptr<Vector3d> new_acc(new Vector3d(transform(requested_vector)));
		requests.insert(boost::shared_ptr<Request>(new AccelerationRequest(*robot, new_acc)));
	}

	void add_position_request(Vector3dWrapper requested_vector) {
		boost::shared_ptr<Vector3d> new_pos(new Vector3d(transform(requested_vector)));
		requests.insert(boost::shared_ptr<Request>(new PositionRequest(*robot, new_pos)));
	}

	void add_velocity_request(Vector3dWrapper requested_vector) {
		boost::shared_ptr<Vector3d> new_vel(new Vector3d(transform(requested_vector)));
		requests.insert(boost::shared_ptr<Request>(new VelocityRequest(*robot, new_vel)));
	}

	void add_marker_request(MarkerInformationWrapper marker) {
		boost::shared_ptr<MarkerInformation> new_marker(new MarkerInformation(transform(marker)));
		requests.insert(boost::shared_ptr<Request>(new MarkerRequest(*robot, new_marker)));
	}

	void add_type_change_request(unsigned type) {
		requests.insert(boost::shared_ptr<Request>(new TypeChangeRequest(*robot, static_cast<RobotType>(type))));
	}

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
	int status = luaL_loadfile(lua_state_.get(), lua_file_name.c_str());
	if(status != 0) {
		report_errors(status);
		throw std::invalid_argument("Error while loading given lua file (" + lua_file_name + ").");
	}
	status = lua_pcall(lua_state_.get(), 0, LUA_MULTRET, 0);
	report_errors(status);
	register_lua_methods();
}

void LuaRobot::register_lua_methods() {
	//register view methods to lua
	luabind::module(lua_state_.get())
	[
		 luabind::class_<Vector3dWrapper>("Vector3d")
			 .def(luabind::constructor<>())
			 .def(luabind::constructor<double, double, double>())
			 .def(luabind::const_self + luabind::other<Vector3dWrapper>())
			 .def(luabind::const_self * luabind::other<Vector3dWrapper>())
			 .def(luabind::const_self / double())
			 .def(luabind::tostring(luabind::self))
			 .def_readwrite("x", &Vector3dWrapper::x)
			 .def_readwrite("y", &Vector3dWrapper::y)
			 .def_readwrite("z", &Vector3dWrapper::z),

		 luabind::class_<MarkerInformationWrapper>("MarkerInformation")
			 .def(luabind::constructor<>())
			 .def("add_data", &MarkerInformationWrapper::add_data)
			 .def("get_data", &MarkerInformationWrapper::get_data),

		 luabind::class_<CoordinateSystemWrapper>("CoordinateSystem")
			 .def(luabind::constructor<>())
			 .def(luabind::constructor<Vector3dWrapper, Vector3dWrapper, Vector3dWrapper>())
			 .def_readwrite("x_axis", &CoordinateSystemWrapper::x_axis)
			 .def_readwrite("y_axis", &CoordinateSystemWrapper::y_axis)
			 .def_readwrite("z_axis", &CoordinateSystemWrapper::z_axis),

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

		 luabind::def("get_visible_robots", &get_visible_robots, luabind::copy_table(luabind::result)),
		 luabind::def("get_visible_obstacles", &get_visible_obstacles, luabind::copy_table(luabind::result)),
		 luabind::def("get_visible_markers", &get_visible_markers, luabind::copy_table(luabind::result)),
		 luabind::def("get_position", &get_position),
		 luabind::def("get_marker_information", &get_marker_information),
		 luabind::def("get_id", &get_id),
		 luabind::def("get_robot_acceleration", &get_robot_acceleration),
		 luabind::def("get_robot_coordinate_system_axis", &get_robot_coordinate_system_axis),
		 luabind::def("get_robot_type", &get_robot_type),
		 luabind::def("get_robot_status", &get_robot_status),
		 luabind::def("is_point_in_obstacle", &is_point_in_obstacle),
		 luabind::def("get_box_depth", &get_box_depth),
		 luabind::def("get_box_width", &get_box_width),
		 luabind::def("get_box_height", &get_box_height),
		 luabind::def("get_sphere_radius", &get_sphere_radius),
		 luabind::def("is_sphere_identifier", &is_sphere_identifier),
		 luabind::def("is_box_identifier", &is_box_identifier),
		 luabind::def("add_acceleration_request", &add_acceleration_request),
		 luabind::def("add_position_request", &add_position_request),
		 luabind::def("add_velocity_request", &add_velocity_request),
		 luabind::def("add_type_change_request", &add_type_change_request),
		 luabind::def("add_marker_request", &add_marker_request),
		 luabind::def("get_own_identifier", &get_own_identifier)
	];
}

std::set<boost::shared_ptr<Request> > LuaRobot::compute() {
	if(view = view_.lock()) {
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
	}
	else {
		throw std::logic_error("Could not lock view. Too old?");
	}
	return requests;
}


std::string LuaRobot::get_algorithm_id () const {
	//TODO (dwonisch): algorithm_id is now either a class name or a lua file.
	return lua_file_name_;
}



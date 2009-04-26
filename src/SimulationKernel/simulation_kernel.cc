#include "simulation_kernel.h"

#include <map>
#include <string>
#include <boost/any.hpp>

#include "../ActivationSequenceGenerators/activation_sequence_generator.h"

#include "../EventHandlers/event_handler.h"
#include "../EventHandlers/marker_request_handler.h"
#include "../EventHandlers/type_change_request_handler.h"

#include "robot_control.h"
#include "../Views/abstract_view_factory.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/box_identifier.h"
#include "../Model/sphere_identifier.h"
#include "../Model/marker_identifier.h"

#include "../RobotImplementations/simple_robot.h"

#include "../Utilities/VectorModifiers/vector_trimmer.h"
#include "../Utilities/VectorModifiers/vector_difference_trimmer.h"
#include "../Utilities/VectorModifiers/vector_randomizer.h"

#include "factories.h"
#include <Utilities/console_output.h>

#include <iostream>


SimulationKernel::SimulationKernel() {
	/* REMARK: If you want to add some Values to the maps, make sure to use only
	 * upper-case letters! All strings read from the projectfiles are converted to upper-case
	 * by boost::to_upper_copy() so it will simply not work if you use lowercase here.
	 */

	// initialize Stati-map
	  robot_status_map_["READY"] = READY;
	  robot_status_map_["SLEEPING"] = SLEEPING;

	// initialize Stati-map
	  robot_type_map_["SLAVE"] = SLAVE;
	  robot_type_map_["MASTER"] = MASTER;
}

SimulationKernel::~SimulationKernel() {

}

const vector<boost::shared_ptr<Robot> >& SimulationKernel::robots() const {
	return robots_;
}

const boost::shared_ptr<History>& SimulationKernel::history() const {
	return history_;
}


void SimulationKernel::init(const string& project_filename, boost::shared_ptr<History> history, std::string output_dir, bool create_statistics) {

	// set history
	history_ = history;

	// create Parser, load project file and get map
	boost::shared_ptr<Parser> parser(new Parser());
	parser->load_projectfiles(project_filename);
	std::map<std::string, std::string> &params = parser->parameter_map();
	ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::info) << "Generated parameter map";
	// create and add initial world information to history
	boost::shared_ptr<WorldInformation> initial_world_information = setup_initial_world_information(parser);

	boost::shared_ptr<TimePoint> initial_time_point(new TimePoint());
	initial_time_point->set_world_information(initial_world_information);
	history_->insert(initial_time_point);

	// create Robot Control
	boost::shared_ptr<RobotControl> robot_control = Factory::robot_control_factory(params, history_->capacity(), initial_world_information);

	// create ASG
	asg_ = Factory::asg_factory(params);
	asg_->initialize(*history_, robots_);

	// create EventHandler.
	event_handler_ = Factory::event_handler_factory(params, history_, robot_control);

	// create and initialize statistics module;
	if (create_statistics==true) {
		stats_.reset(new StatsControl());
		stats_->init(params, output_dir);
	}
	else
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Output disabled.";

	// register SimulationObservers (ViewObject, ASG, maybe StatisticObject)
	event_handler_->register_listener(asg_);
	if (create_statistics==true)
		event_handler_->register_listener(stats_);

	// send initial worldinformation to statistics
	if (create_statistics==true) {
		boost::shared_ptr<Event> foo = boost::shared_ptr<Event>();
		stats_->update(*(initial_world_information.get()), foo);
	}

	camera_position_ = params["CAMERA_POSITION"];
	camera_direction_ = params["CAMERA_DIRECTION"];
}


void SimulationKernel::step() {

	event_handler_->handle_event(asg_->get_next_event());
	//std::cout << "History Size: " << history_->size() << std::endl;
}


void SimulationKernel::multistep(int steps) {
	for (int i = 0; i < steps; i++) {
		step();
	}
}


void SimulationKernel::quit() {
	if (stats_!=NULL)
		stats_->quit();
}

void SimulationKernel::create_robots(boost::shared_ptr<Parser> parser, boost::shared_ptr<WorldInformation> initial_world_information) {

	boost::shared_ptr<RobotData> temp_robot_data;
	boost::shared_ptr<Robot> temp_robot;
	boost::shared_ptr<RobotIdentifier> temp_robot_identifier;
	std::string temp_robot_algorithm;

	boost::shared_ptr<Vector3d> temp_robot_position;
	boost::shared_ptr<Vector3d> temp_robot_velocity;
	boost::shared_ptr<Vector3d> temp_robot_acceleration;


	boost::tuple <boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d>, boost::shared_ptr<Vector3d> > temp_robot_axes;

	// counting robots by their position vectors
	vector<Vector3d>::size_type robot_count;
	robot_count = parser->robot_positions().size();

	// running through all robots in robotfile
	for (vector<Vector3d>::size_type i = 0; i < robot_count; i++) {
		//TODO(mmarcus) Identifiers are NOT the identifiers of the project file!
		//need to work over parser...
		temp_robot_identifier.reset(new RobotIdentifier(i));
		temp_robot_algorithm = parser->robot_algorithms()[i];

		temp_robot = Factory::robot_factory(temp_robot_identifier, temp_robot_algorithm);

		temp_robot_position.reset(new Vector3d(parser->robot_positions()[i]));
		temp_robot_velocity.reset(new Vector3d(parser->robot_velocities()[i]));
		temp_robot_acceleration.reset(new Vector3d(parser->robot_accelerations()[i]));

		temp_robot_data.reset(new RobotData(temp_robot_identifier, temp_robot_position, *temp_robot));

		temp_robot_data->set_velocity(temp_robot_velocity);
		temp_robot_data->set_acceleration(temp_robot_acceleration);

		temp_robot_data->set_type(robot_type_map_[boost::to_upper_copy(parser->robot_types()[i])]);
		temp_robot_data->set_status(robot_status_map_[boost::to_upper_copy(parser->robot_stati()[i])]);

		temp_robot_data->set_color(boost::lexical_cast<int>(parser->robot_colors()[i]));

		boost::get<0>(temp_robot_axes).reset(new Vector3d(boost::get<0>(parser->robot_coordinate_systems()[i])));
		boost::get<1>(temp_robot_axes).reset(new Vector3d(boost::get<1>(parser->robot_coordinate_systems()[i])));
		boost::get<2>(temp_robot_axes).reset(new Vector3d(boost::get<2>(parser->robot_coordinate_systems()[i])));

		temp_robot_data->set_coordinate_system_axis(temp_robot_axes);

		robots_.push_back(temp_robot);
		initial_world_information->add_robot_data(temp_robot_data);
	}

}

void SimulationKernel::create_obstacles_and_marker(boost::shared_ptr<Parser> parser, boost::shared_ptr<WorldInformation> initial_world_information) {

	//some temporary variables
	boost::shared_ptr<BoxIdentifier> tmp_box_identifier;
	boost::shared_ptr<SphereIdentifier> tmp_sphere_identifier;
	boost::shared_ptr<MarkerIdentifier> tmp_marker_identifier;

	boost::shared_ptr<Sphere> tmp_sphere;
	boost::shared_ptr<Box> tmp_box;
	boost::shared_ptr<WorldObject> tmp_marker;

	boost::shared_ptr<Vector3d> tmp_pos;
	boost::shared_ptr<MarkerInformation> tmp_marker_information;

	std::vector<boost::shared_ptr<Obstacle> > obstacles_vector;
	std::vector<boost::shared_ptr<WorldObject> > markers_vector;

	//get initial obstacle information read from input file
	std::vector<string> obstacle_types = parser->obstacle_types();
	std::vector<Vector3d> obstacle_positions = parser->obstacle_positions();
	std::vector<string> obstacle_marker_information = parser->obstacle_marker_information();
	std::vector<double> obstacle_radius = parser->obstacle_radius();
	std::vector<Vector3d> obstacle_size = parser->obstacle_size();

	//count number of markers
	unsigned num_of_marker = 0;
	unsigned num_of_obstacles = 0;

	obstacles_vector.reserve(obstacle_types.size());
	markers_vector.reserve(obstacle_types.size());

	//iterate through all obstacles
	for(unsigned i=0; i<obstacle_types.size(); ++i) {

		//get position
		tmp_pos.reset(new Vector3d());
		tmp_pos->insert_element(kXCoord, obstacle_positions[i](0));
		tmp_pos->insert_element(kYCoord, obstacle_positions[i](1));
		tmp_pos->insert_element(kZCoord, obstacle_positions[i](2));

		//get marker information
		tmp_marker_information.reset(new MarkerInformation());
		//TODO(martinah) Maybe adapt variable name "MarkerInfoFromInputFile"
		tmp_marker_information->add_data("MarkerInfoFromInputFile", obstacle_marker_information[i]);

		//get type of obstacle
		if(!obstacle_types[i].compare("box")) {

			//create according identifier
			tmp_box_identifier.reset(new BoxIdentifier(num_of_obstacles++));

			tmp_box.reset(new Box(
					tmp_box_identifier,
					tmp_pos,
					tmp_marker_information,
					obstacle_size[i](2),	//z-length = depth
					obstacle_size[i](0),	//x-length = width
					obstacle_size[i](1)		//y-length = height
					));

			//add obstacle to vector of obstacles
			obstacles_vector.push_back(tmp_box);

		} else if(!obstacle_types[i].compare("sphere")) {

			//create according identifier
			tmp_sphere_identifier.reset(new SphereIdentifier(num_of_obstacles++));

			tmp_sphere.reset(new Sphere(
					tmp_sphere_identifier,
					tmp_pos,
					tmp_marker_information,
					obstacle_radius[i]
					));

			//add obstacle to vector of obstacles
			obstacles_vector.push_back(tmp_sphere);

		} else if(!obstacle_types[i].compare("marker")) {

			//create according identifier
			tmp_marker_identifier.reset(new MarkerIdentifier(num_of_marker++));

			tmp_marker.reset(new WorldObject(
						tmp_marker_identifier,
						tmp_pos,
						tmp_marker_information
						));

			//add marker to vector of markers
			markers_vector.push_back(tmp_marker);


		}
	}
	//add vector of obstacles to the world information
	initial_world_information->set_obstacle_data(obstacles_vector);
	initial_world_information->set_marker_data(markers_vector);
}

boost::shared_ptr<WorldInformation> SimulationKernel::setup_initial_world_information(
		boost::shared_ptr<Parser> parser) {

	//create new WorldInformation
	boost::shared_ptr<WorldInformation> initial_world_information(new WorldInformation());

	//create robots and add to initial world information
	create_robots(parser, initial_world_information);

	//create obstacles and marker and add to initial world information
	create_obstacles_and_marker(parser, initial_world_information);

	//set time of initial world information
	initial_world_information->set_time(0);

	return initial_world_information;
}

boost::shared_ptr<RequestHandler> SimulationKernel::setup_request_handler(RequestHandlerType req_type,
																		  unsigned int seed,
																		  double discard_prob,
																		  boost::shared_ptr<History> history,
																		  vector<string> vector_modifiers) {

	boost::shared_ptr<RequestHandler> request_handler;
		switch (req_type) {
			case POSITION_REQUEST_HANDLER:
			case VELOCITY_REQUEST_HANDLER:
			case ACCELERATION_REQUEST_HANDLER:
				request_handler.reset(new VectorRequestHandler(seed, discard_prob, *history));
				setup_vectormodifier(boost::dynamic_pointer_cast<VectorRequestHandler>(request_handler), vector_modifiers);
				break;
			case TYPE_CHANGE_REQUEST_HANDLER:
				request_handler.reset(new TypeChangeRequestHandler(seed, discard_prob, *history));
				break;
			case MARKER_REQUEST_HANDLER:
				request_handler.reset(new MarkerRequestHandler(seed, discard_prob, *history));
				break;
		}

		return request_handler;
}

void SimulationKernel::setup_vectormodifier(boost::shared_ptr<VectorRequestHandler> request_handler,
																		 vector<string> vector_modifiers) {

	boost::shared_ptr<VectorModifier> vecmod;

	//TODO(mmarcus) here are a lot of dummy values... need to find solution for parametrized vector modifiers
	BOOST_FOREACH( string s, vector_modifiers) {
		switch (vector_modifier_map_[boost::to_upper_copy(s)]) {
			case VECTOR_TRIMMER:
				vecmod.reset(new VectorTrimmer(42.0));
				request_handler->add_vector_modifier(vecmod);
				break;
				vecmod.reset(new VectorDifferenceTrimmer(42.0));
				request_handler->add_vector_modifier(vecmod);
				break;
			case VECTOR_RANDOMIZER:
				vecmod.reset(new VectorRandomizer(1, 42.0));
				request_handler->add_vector_modifier(vecmod);
				break;
		}
	}
}

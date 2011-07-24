#include "simulation_kernel.h"

#include <map>
#include <string>
#include <boost/any.hpp>

#include "../ActivationSequenceGenerators/activation_sequence_generator.h"

#include "../EventHandlers/event_handler.h"
#include "../EventHandlers/marker_request_handler.h"
#include "../EventHandlers/type_change_request_handler.h"
#include "../EventHandlers/color_change_request_handler.h"

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

using namespace std;

// Radius below which robots are counted as being on the same spot
// for multiplicity detection
double kEpsilonRadius = 0.001;

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


void SimulationKernel::init(const string& project_filename,
		                    boost::shared_ptr<History> history,
		                    std::string output_dir,
		                    bool create_statistics) {

	// set history
	history_ = history;

	// create Parser, load project file and get map
	parser_.reset(new Parser());
	parser_->load_projectfiles(project_filename);
	std::map<std::string, std::string> &params = parser_->parameter_map();
	ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::info) << "Generated parameter map";
	// create and add initial world information to history
	boost::shared_ptr<WorldInformation> initial_world_information = setup_initial_world_information(parser_);

	boost::shared_ptr<TimePoint> initial_time_point(new TimePoint());
	initial_time_point->set_world_information(initial_world_information);
	history_->insert(initial_time_point);

	// create Robot Control
	boost::shared_ptr<RobotControl> robot_control = Factory::robot_control_factory(params, history_->capacity(), initial_world_information);

	// create ASG
	asg_ = Factory::asg_factory(params);
	asg_->initialize(*history_, robots_, world_modifiers_);

	// create EventHandler.
	event_handler_ = Factory::event_handler_factory(params, history_, robot_control);

	// create and initialize statistics module;
	if (create_statistics==true) {
		stats_.reset(new StatsControl());
		stats_->init(params, output_dir);
	} else {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Output disabled.";

		// workaround for visibility-graph in --dry-mode:
		// create and register always a statistics object,
		// but here we do not initialize it => no output
		stats_.reset(new StatsControl());
	}

	// register SimulationObservers (ViewObject, ASG, maybe StatisticObject)
	event_handler_->register_listener(asg_);
	if (create_statistics==true)
		event_handler_->register_listener(stats_);

	// send initial worldinformation to statistics
	if (create_statistics==true) {
		boost::shared_ptr<Event> foo = boost::shared_ptr<Event>();
		stats_->update(*initial_time_point, foo);
	}

	camera_position_ = params["CAMERA_POSITION"];
	camera_direction_ = params["CAMERA_DIRECTION"];
	camera_type_ = params["CAMERA_TYPE"];
}


void SimulationKernel::step() {
	// create a new time point
	boost::shared_ptr<TimePoint> time_point(new TimePoint());

	// fill time point
	event_handler_->handle_event(asg_->get_next_event(), *time_point);

	// insert time point
	history_->insert(time_point);
}


void SimulationKernel::multistep(int steps) {
	for (int i = 0; i < steps; i++) {
		step();
	}
}


void SimulationKernel::quit() {
	last_breath();
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

void SimulationKernel::create_world_modifiers(boost::shared_ptr<Parser> parser) {
	boost::shared_ptr<WorldModifier> temp_world_modifier;
	std::string temp_world_modifier_name;

	vector<string>& temp_world_modifiers = parser->world_modifiers();

	for (int i = 0; i < temp_world_modifiers.size(); i++) {
		temp_world_modifier_name = temp_world_modifiers[i];
		temp_world_modifier = Factory::world_modifier_factory(temp_world_modifier_name);



		world_modifiers_.push_back(temp_world_modifier);
	}
}

boost::shared_ptr<WorldInformation> SimulationKernel::setup_initial_world_information(
		boost::shared_ptr<Parser> parser) {

	//create new WorldInformation
	boost::shared_ptr<WorldInformation> initial_world_information(new WorldInformation());

	//create robots and add to initial world information
	create_robots(parser, initial_world_information);

	//create obstacles and marker and add to initial world information
	create_obstacles_and_marker(parser, initial_world_information);

	//create world modifiers and add to initial world information
	create_world_modifiers(parser);

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
			case COLOR_REQUEST_HANDLER:
				request_handler.reset(new ColorChangeRequestHandler(seed,discard_prob,*history));
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
			case VECTOR_DIFFERENCE_TRIMMER:
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

void SimulationKernel::dump_simulation() {
	// create a Parser to save actual robotconfiguration
	std::string dumpnumber = boost::lexical_cast<std::string>(parser_->dumpnumber());
	parser_->set_project_filename(("dump_" + dumpnumber).c_str());
	parser_->set_robot_filename(("dump_" + dumpnumber).c_str());
	parser_->set_obstacle_filename(("dump_" + dumpnumber).c_str());
	parser_->save_projectfiles(("dump_" + dumpnumber).c_str(), history_->get_newest().world_information());
}

bool SimulationKernel::terminate_condition(bool run_until_no_multiplicity) const {
	bool result = false;
	if (run_until_no_multiplicity) {
		const TimePoint& newest = history_->get_newest();
		const WorldInformation& world_info = newest.world_information();
		const std::vector<boost::shared_ptr<RobotData> >& robot_data_vec =
		    world_info.robot_data();

		// This is a very naive approach.
		bool no_multiplicity = true;
		BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data, robot_data_vec) {
			BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data_b, robot_data_vec) {
				if(&robot_data->robot() != &robot_data_b->robot()) {
					double distance = vector3d_distance(robot_data->position(),
							                            robot_data_b->position());
					if (distance < kEpsilonRadius) {
						no_multiplicity = false;
						break;
					}

				}
			}
		}
		result = result || no_multiplicity;
	}

	return result;
}

void SimulationKernel::last_breath() const {
	std::cout << "Last breath start" << std::endl;
	const TimePoint& newest = history_->get_newest();
	const WorldInformation& world_info = newest.world_information();
	const std::vector<boost::shared_ptr<RobotData> >& robot_data_vec =
	    world_info.robot_data();

	double max_dist_x = 0.0;
	double max_dist_y = 0.0;
	double max_dist_z = 0.0;

	// Computing the Regularity metric.
	// This is a very naive approach.
	BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data, robot_data_vec) {
		BOOST_FOREACH(boost::shared_ptr<RobotData> robot_data_b, robot_data_vec) {
			if(&robot_data->robot() != &robot_data_b->robot()) {
				double dist_x = (robot_data->position())(kXCoord) -
				                (robot_data_b->position())(kXCoord) + 1;
				if(dist_x < 0) dist_x *= -1;
				max_dist_x = max(max_dist_x, dist_x);

				double dist_y = (robot_data->position())(kYCoord) -
								(robot_data_b->position())(kYCoord) + 1;
				if(dist_y < 0) dist_y *= -1;
				max_dist_y = max(max_dist_y, dist_y);

				double dist_z = (robot_data->position())(kZCoord) -
				                (robot_data_b->position())(kZCoord) + 1;
				if(dist_z < 0) dist_z *= -1;
				max_dist_z = max(max_dist_z, dist_z);
			}
		}
	}

	// This differs by the regularity metric as defined in
	// "Local lattice construction and the Flow algorithm"
	// but only by a constant factor c which is good enough for us.
	double regularity = max(max_dist_x, max_dist_y);
	double gaps = max_dist_x * max_dist_y - robot_data_vec.size();
	regularity = max(regularity, max_dist_z);
	std::cout << std::endl << "REGULARITY: " << regularity << std::endl;
	std::cout << std::endl << "GAPS: " << gaps << std::endl;
}


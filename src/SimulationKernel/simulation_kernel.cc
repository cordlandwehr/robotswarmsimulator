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

#include "../RobotImplementations/simple_robot.h"

#include "../Utilities/VectorModifiers/vector_trimmer.h"
#include "../Utilities/VectorModifiers/vector_difference_trimmer.h"
#include "../Utilities/VectorModifiers/vector_randomizer.h"

#include "factories.h"

#include <iostream>


SimulationKernel::SimulationKernel() {
	/* REMARK: If you want to add some Values to the maps, make sure to use only
	 * upper-case letters! All strings read from the projectfiles are converted to upper-case
	 * by boost::to_upper_copy() so it will simply not work if you use lowercase here.
	 */

	// initialize view-map
	  view_map_["FULLVIEW"] = FULLVIEW;
	  view_map_["GLOBALVIEW"] = GLOBALVIEW;

	// initialize Stati-map
	  robot_status_map_["READY"] = READY;
	  robot_status_map_["SLEEPING"] = SLEEPING;

	// initialize Stati-map
	  robot_type_map_["SLAVE"] = SLAVE;
	  robot_type_map_["MASTER"] = MASTER;

	// initialize Vector modifier-map
	  vector_modifier_map_["VECTOR_TRIMMER"] = VECTOR_TRIMMER;
	  vector_modifier_map_["VECTOR_RANDOMIZER"] = VECTOR_RANDOMIZER;
	  vector_modifier_map_["VECTOR_DIFFERENCE_TRIMMER"] = VECTOR_DIFFERENCE_TRIMMER;
}

SimulationKernel::~SimulationKernel() {
	// TODO Auto-generated destructor stub
}

const vector<boost::shared_ptr<Robot> >& SimulationKernel::robots() const {
	return robots_;
}

const boost::shared_ptr<History>& SimulationKernel::history() const {
	return history_;
}


void SimulationKernel::init(const string& project_filename, boost::shared_ptr<History> history) {

	// set history
	history_ = history;

	// create Parser, load project file and get map
	boost::shared_ptr<Parser> parser(new Parser());
	parser->load_projectfiles(project_filename);
	// TODO(craupach) since this is not implemented yet the simulation segfaults. known problem.
	std::map<std::string, boost::any> &params = parser->get_parameter_map();

	// create and add initial world information to history
	boost::shared_ptr<WorldInformation> initial_world_information = setup_initial_world_information(parser);
	history_->insert(initial_world_information);


	// create View
	boost::shared_ptr<AbstractViewFactory> view_factory = Factory::view_factory_factory(params);

	// create Robot Control
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, history_->capacity(), *initial_world_information));

	// create ASG
	asg_ = Factory::asg_factory(params);
	asg_->initialize(*history_, robots_);

	// create EventHandler.
	event_handler_ = Factory::event_handler_factory(params, history_, robot_control);

	// create and initialize statistics module;
	stats_.reset(new StatsControl());
	stats_->init(parser);

	// register SimulationObservers (ViewObject, ASG, maybe StatisticObject)
	//TODO(Sven) "stats_->update(initialWorldInformation)"
	event_handler_->register_listener(asg_);
	event_handler_->register_listener(stats_);

}


void SimulationKernel::step() {
	event_handler_->handle_event(asg_->get_next_event());
}


void SimulationKernel::multistep(int steps) {
	for (int i = 0; i < steps; i++) {
		step();
	}
}


void SimulationKernel::quit() {
	stats_->quit();
}


boost::shared_ptr<WorldInformation> SimulationKernel::setup_initial_world_information(
		boost::shared_ptr<Parser> parser) {

	// taking some Pointers to point to the right information
	boost::shared_ptr<WorldInformation> initial_world_information(new WorldInformation());

	boost::shared_ptr<RobotData> temp_robot_data;
	boost::shared_ptr<Robot> temp_robot;
	boost::shared_ptr<RobotIdentifier> temp_robot_identifier;
	boost::shared_ptr<std::string> temp_robot_algorithm;

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
		temp_robot_algorithm.reset(new std::string(parser->robot_algorithms()[i]));

		//TODO(mmarcus) maybe this is the right place to check for the algorithm?
		//different algorithms = different RobotTypes?
		//TODO(cola) lua-robots need to be included.. simple way: check if file is of type *.lua ?!
		temp_robot.reset(new SimpleRobot(temp_robot_identifier, temp_robot_algorithm));

		temp_robot_position.reset(new Vector3d(parser->robot_positions()[i]));
		temp_robot_velocity.reset(new Vector3d(parser->robot_velocities()[i]));
		temp_robot_acceleration.reset(new Vector3d(parser->robot_accelerations()[i]));

		temp_robot_data.reset(new RobotData(temp_robot_identifier, temp_robot_position, *temp_robot));

		temp_robot_data->set_velocity(temp_robot_velocity);
		temp_robot_data->set_acceleration(temp_robot_acceleration);

		temp_robot_data->set_type(robot_type_map_[boost::to_upper_copy(parser->robot_types()[i])]);
		temp_robot_data->set_status(robot_status_map_[boost::to_upper_copy(parser->robot_stati()[i])]);

		boost::get<0>(temp_robot_axes).reset(new Vector3d(boost::get<0>(parser->robot_coordinate_systems()[i])));
		boost::get<1>(temp_robot_axes).reset(new Vector3d(boost::get<1>(parser->robot_coordinate_systems()[i])));
		boost::get<2>(temp_robot_axes).reset(new Vector3d(boost::get<2>(parser->robot_coordinate_systems()[i])));

		temp_robot_data->set_coordinate_system_axis(temp_robot_axes);

		robots_.push_back(temp_robot);
		initial_world_information->add_robot_data(temp_robot_data);
	}

	// set time of inital world information
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

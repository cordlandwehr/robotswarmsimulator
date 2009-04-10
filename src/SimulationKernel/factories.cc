/*
 * factories.cc
 *
 *  Created on: Feb 22, 2009
 *      Author: craupach
 */

#include "factories.h"
#include <string>
#include <boost/any.hpp>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>

#include "../ActivationSequenceGenerators/activation_sequence_generator.h"
#include "../ActivationSequenceGenerators/synchronous_asg.h"
#include "../ActivationSequenceGenerators/atomic_semisynchronous_asg.h"
#include "../ActivationSequenceGenerators/asynchronous_asg.h"

#include "../Views/abstract_view_factory.h"
#include "../Views/view_factory.h"
#include "../Views/parametrized_view_factory.h"
#include "../ViewModels/global_view.h"
#include "../ViewModels/cog_view.h"
#include "../ViewModels/one_point_formation_view.h"
#include "../ViewModels/chain_view.h"
#include "../ViewModels/self_view.h"

#include "../EventHandlers/event_handler.h"
#include "../EventHandlers/marker_request_handler.h"
#include "../EventHandlers/type_change_request_handler.h"
#include "../EventHandlers/vector_request_handler.h"
#include "../EventHandlers/marker_change_request_handler.h"
#include "../EventHandlers/collision_position_request_handler.h"

#include "../Utilities/VectorModifiers/vector_modifier.h"
#include "../Utilities/VectorModifiers/vector_difference_trimmer.h"
#include "../Utilities/VectorModifiers/vector_trimmer.h"
#include "../Utilities/VectorModifiers/vector_randomizer.h"
#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/console_output.h"
#include "../Utilities/distribution_generator.h"

#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../RobotImplementations/lua_robot.h"
#include "../RobotImplementations/simple_robot.h"
#include "../RobotImplementations/cog_robot.h"
#include "../RobotImplementations/velocity_cog_robot.h"
#include "../RobotImplementations/acceleration_cog_robot.h"
#include "../RobotImplementations/miniball_robot.h"
#include "../RobotImplementations/tp_algorithm_robot.h"

#include "../SimulationKernel/robot_control.h"
#include "../SimulationKernel/uniform_robot_control.h"
#include "../SimulationKernel/robot_type_robot_control.h"
#include "../Model/world_information.h"
#include "../Model/robot_data.h"

#include <iostream>

/**
 * creates vector modfiers from the given string and adds them to the handler
 * TODO(craupach) proper error handling
 */
void create_vector_modifiers_from_string(boost::shared_ptr<VectorRequestHandler> handler, const std::string& modifiers ) {
	// set up boost string tokenizer for ";" (delimeter between vector modifiers)
	boost::char_separator<char> sep(";");
	boost::tokenizer<boost::char_separator<char> > tokenizer(modifiers, sep);

	// iterate through all substrings (each represents a single modifier)
	BOOST_FOREACH(std::string modifier, tokenizer) {
		// discard leading and trailing '(' ')'
		boost::trim_if(modifier, boost::is_any_of("()"));

		// split modifier string
		boost::char_separator<char> inner_sep(",");
		boost::tokenizer<boost::char_separator<char> > inner_tokenizer(modifier, inner_sep);

		// get first token (type of vector modifier)
		boost::tokenizer<boost::char_separator<char> >::iterator param_iterator = inner_tokenizer.begin();
		std::string type = *param_iterator;
		++param_iterator;

		// switch through types. This is the place init code for new modifiers should be inserted.
		boost::shared_ptr<VectorModifier> new_vector_modifier;
		try {
			if(type == "VECTOR_TRIMMER") {
				double length = boost::lexical_cast<double>(*param_iterator);
				new_vector_modifier.reset(new VectorTrimmer(length));
			} else if(type == "VECTOR_DIFFERENCE_TRIMMER") {
				double length = boost::lexical_cast<double>(*param_iterator);
				new_vector_modifier.reset(new VectorDifferenceTrimmer(length));
			} else if(type == "VECTOR_RANDOMIZER") {
				unsigned int seed = boost::lexical_cast<unsigned int>(*param_iterator);
				++param_iterator;
				double standard_deviation = boost::lexical_cast<double>(*param_iterator);
				new_vector_modifier.reset(new VectorRandomizer(seed, standard_deviation));
			}
		} catch(const boost::bad_lexical_cast& ) {
			 throw UnsupportedOperationException("Failed creating proper vector modifiers when parsing " + modifiers);
		}
		handler->add_vector_modifier(new_vector_modifier);
	}
}


boost::shared_ptr<EventHandler> Factory::event_handler_factory(std::map<std::string, std::string> &params,
                                                               boost::shared_ptr<History> history,
                                                               boost::shared_ptr<RobotControl> robot_control) {
	boost::shared_ptr<EventHandler> event_handler(new EventHandler(history, robot_control));

	// create the request handlers from the parameters

	// 1. Marker Request Handler
	std::string marker_request_handler_type = params["MARKER_REQUEST_HANDLER_TYPE"];
	if(marker_request_handler_type == "STANDARD") {
			try {
				// build the marker request handler
				double discard_probability = boost::lexical_cast<double> (params["STANDARD_MARKER_REQUEST_HANDLER_DISCARD_PROB"]);
				unsigned int seed = boost::lexical_cast<unsigned int> (params["STANDARD_MARKER_REQUEST_HANDLER_SEED"]);
				boost::shared_ptr<MarkerRequestHandler> marker_request_handler(new MarkerRequestHandler(seed, discard_probability, *history));
				event_handler->set_marker_request_handler(marker_request_handler);
			} catch(const boost::bad_lexical_cast& ) {
				throw UnsupportedOperationException("Failed reading parameters for standard marker request handler");
			}
	}

	// 2. Type Change Request Handler
	std::string type_change_request_handler_type = boost::any_cast<std::string> (params["TYPE_CHANGE_REQUEST_HANDLER_TYPE"]);
	if(type_change_request_handler_type == "STANDARD") {
		try {
			// build the type change request handler
			double discard_probability = boost::lexical_cast<double> (params["STANDARD_TYPE_CHANGE_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = boost::lexical_cast<unsigned int> (params["STANDARD_TYPE_CHANGE_REQUEST_HANDLER_SEED"]);
			boost::shared_ptr<TypeChangeRequestHandler> type_change_request_handler(new TypeChangeRequestHandler(seed, discard_probability, *history));
			event_handler->set_type_change_request_handler(type_change_request_handler);
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for standard type change request handler");
		}
	}

	// 3. Position Request Handler
	std::string position_request_handler_type = boost::any_cast<std::string> (params["POSITION_REQUEST_HANDLER_TYPE"]);
	if(position_request_handler_type == "VECTOR") {
		try {
			// build the vector request handler
			double discard_probability = boost::lexical_cast<double> (params["VECTOR_POSITION_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = boost::lexical_cast<unsigned int> (params["VECTOR_POSITION_REQUEST_HANDLER_SEED"]);
			boost::shared_ptr<VectorRequestHandler> vector_request_handler(new VectorRequestHandler(seed, discard_probability, *history));

			// set up vector modifiers
			create_vector_modifiers_from_string(vector_request_handler,
					                            params["VECTOR_POSITION_REQUEST_HANDLER_MODIFIER"]);

			event_handler->set_position_request_handler(vector_request_handler);
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for vector position request handler");
		}
	} else if (position_request_handler_type == "COLLISION") {
		try {
			// get parameters for the collision position request handler
			std::string collision_strategy = params["COLLISION_POSITION_REQUEST_HANDLER_STRATEGY"];
			CollisionPositionRequestHandler::CollisionStrategy strategy;
			if (collision_strategy == "STOP")
				strategy = CollisionPositionRequestHandler::STOP;
			else if (collision_strategy == "TOUCH")
				strategy = CollisionPositionRequestHandler::TOUCH;
			else {
				strategy = CollisionPositionRequestHandler::STOP;
				ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::warning)  << "Unknown collision strategy, falling back to STOP strategy.";
			}

			double clearance = boost::lexical_cast<double>(params["COLLISION_POSITION_REQUEST_HANDLER_CLEARANCE"]);
			double discard_probability = boost::lexical_cast<double> (params["COLLISION_POSITION_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = boost::lexical_cast<unsigned int> (params["COLLISION_POSITION_REQUEST_HANDLER_SEED"]);

			// build the collision position request handler
			boost::shared_ptr<CollisionPositionRequestHandler> collisionpos_request_handler(
				new CollisionPositionRequestHandler(strategy, clearance, seed, discard_probability, *history)
			);

			// set up vector modifiers
			create_vector_modifiers_from_string(collisionpos_request_handler,
					                            params["COLLISION_POSITION_REQUEST_HANDLER_MODIFIER"]);

			event_handler->set_position_request_handler(collisionpos_request_handler);
		} catch (const boost::bad_lexical_cast&) {
			throw UnsupportedOperationException("Failed reading parameters for collision position request handler");
		}
	}

	// 4. Velocity Request Handler
	std::string velocity_request_handler_type = boost::any_cast<std::string> (params["VELOCITY_REQUEST_HANDLER_TYPE"]);
	if(velocity_request_handler_type == "VECTOR") {
		try {
			// build the vector request handler
			double discard_probability = boost::lexical_cast<double> (params["VECTOR_VELOCITY_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = boost::lexical_cast<unsigned int> (params["VECTOR_VELOCITY_REQUEST_HANDLER_SEED"]);
			boost::shared_ptr<VectorRequestHandler> vector_request_handler(new VectorRequestHandler(seed, discard_probability, *history));

			// set up vector modifiers
			create_vector_modifiers_from_string(vector_request_handler,
					                            params["VECTOR_VELOCITY_REQUEST_HANDLER_MODIFIER"]);
			event_handler->set_velocity_request_handler(vector_request_handler);
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for vector velocity request handler");
		}
	}

	// 5. Acceleration Request Handler
	std::string acceleration_request_handler_type = boost::any_cast<std::string> (params["ACCELERATION_REQUEST_HANDLER_TYPE"]);
	if(acceleration_request_handler_type == "VECTOR") {
		try {
			// build the vector request handler
			double discard_probability = boost::lexical_cast<double> (params["VECTOR_ACCELERATION_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = boost::lexical_cast<unsigned int> (params["VECTOR_ACCELERATION_REQUEST_HANDLER_SEED"]);
			boost::shared_ptr<VectorRequestHandler> vector_request_handler(new VectorRequestHandler(seed, discard_probability, *history));

			// set up vector modifiers
			create_vector_modifiers_from_string(vector_request_handler,
					                            params["VECTOR_ACCELERATION_REQUEST_HANDLER_MODIFIER"]);
			event_handler->set_acceleration_request_handler(vector_request_handler);
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for vector acceleration request handler");
		}
	}

	// 6. Marker Change Request Handler
	std::string marker_change_request_handler_type = boost::any_cast<std::string> (params["MARKER_CHANGE_REQUEST_HANDLER_TYPE"]);
	if(marker_change_request_handler_type == "STANDARD") {
		try {
			// build the type change request handler
			double discard_probability = boost::lexical_cast<double> (params["STANDARD_MARKER_CHANGE_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = boost::lexical_cast<unsigned int> (params["STANDARD_MARKER_CHANGE_REQUEST_HANDLER_SEED"]);
			boost::shared_ptr<MarkerChangeRequestHandler> marker_change_request_handler(new MarkerChangeRequestHandler(seed, discard_probability, *history));
			event_handler->set_marker_change_request_handler(marker_change_request_handler);
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for standard marker change request handler");
		}
	}

	return event_handler;
}

boost::shared_ptr<ActivationSequenceGenerator> Factory::asg_factory(std::map<std::string, std::string> &params) {

	std::string asg_type = params["ASG"];
	boost::shared_ptr<ActivationSequenceGenerator> asg;
	// setup of activation sequence generator
	if(asg_type == "SYNCHRONOUS") {
			asg.reset(new SynchronousASG());
	} else if(asg_type == "ATOMIC_SEMISYNCHRONOUS") {
		try {
			unsigned int seed = boost::lexical_cast<unsigned int>(params["ATOMIC_SEMISYNC_ASG_SEED"]);
			asg.reset(new AtomicSemisynchronousASG(seed));
		} catch(const boost::bad_lexical_cast&) {
			throw UnsupportedOperationException("Failed reading parameters for atomic semisynchronous asg.");
		}
	} else if(asg_type == "ASYNCHRONOUS") {
		try {
			unsigned int seed = boost::lexical_cast<unsigned int>(params["ASYNC_ASG_SEED"]);
			double participation_probability = boost::lexical_cast<double>(params["ASYNC_ASG_PART_P"]);
			double p = boost::lexical_cast<double>(params["ASYNC_ASG_TIME_P"]);
			asg.reset(new AsynchronousASG(seed, participation_probability, p));
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for asynchronous asg.");
		}
	} else {
		throw UnsupportedOperationException("No ASG specified!");
	}

	return asg;
}

boost::shared_ptr<AbstractViewFactory> Factory::view_factory_factory(std::map<std::string, std::string> &params, const std::string& prefix) {

	std::string view_type = params[prefix + "VIEW"];
	view_type.erase(0,prefix.length()); //erase prefix
	boost::shared_ptr<AbstractViewFactory> view_factory;

	if(view_type == "GLOBAL_VIEW") {
		view_factory.reset(new ViewFactory<GlobalView>);
	} else if(view_type == "COG_VIEW") {
		view_factory.reset(new ViewFactory<CogView>);
	} else if(view_type == "SELF_VIEW") {
		view_factory.reset(new ViewFactory<SelfView>);
	} else if(view_type == "CHAIN_VIEW") {
		try {
			unsigned int number = boost::lexical_cast<unsigned int>(params[prefix+"CHAIN_VIEW_NUM_ROBOTS"]);
			view_factory.reset(new ParametrizedViewFactory<ChainView, unsigned int>(number));
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for chain view.");
		}
	} else if(view_type == "ONE_POINT_FORMATION_VIEW") {
		try {
			double radius = boost::lexical_cast<double>(params[prefix+"ONE_POINT_FORMATION_VIEW_RADIUS"]);
			view_factory.reset(new ParametrizedViewFactory<OnePointFormationView, double>(radius));
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for one point formation view.");
		}
	} else {
		ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::info) << "No View specified! Defaulting to global view.";
		view_factory.reset(new ViewFactory<GlobalView>);
	}

	return view_factory;
}

boost::shared_ptr<Robot> Factory::robot_factory(boost::shared_ptr<RobotIdentifier> id, const std::string& algorithm) {

	boost::shared_ptr<Robot> robot;

	// check if the algorithm is a lua file
	std::string subfix;
	if(algorithm.size() >= 4)
		subfix = algorithm.substr(algorithm.size() - 4, 4);
	if(subfix == ".lua") {
		robot.reset(new LuaRobot(id, algorithm));
	} else if(algorithm == "SimpleRobot" || algorithm == "NONE") {
		robot.reset(new SimpleRobot(id));
	} else if(algorithm == "COGRobot" || algorithm == "TPAlgorithmCOG") {
		robot.reset(new TPAlgorithmRobot(id, TPAlgorithmRobot::cog));
	} else if(algorithm == "VelocityCOGRobot") {
		robot.reset(new VelocityCOGRobot(id));
	} else if(algorithm == "AccelerationCOGRobot") {
	    robot.reset(new AccelerationCOGRobot(id));
	} else if(algorithm == "MiniballRobot" || algorithm == "TPAlgorithmCMinball") {
	    robot.reset(new TPAlgorithmRobot(id, TPAlgorithmRobot::cminball));
	} else if (algorithm == "TPAlgorithmCBox") {
		robot.reset(new TPAlgorithmRobot(id, TPAlgorithmRobot::cbox));
	} else {
		throw UnsupportedOperationException("Tried to create unkown robot type: "+algorithm);
	}
	return robot;
}

boost::shared_ptr<RobotControl> Factory::robot_control_factory(std::map<std::string, std::string> &params, std::size_t history_length, const boost::shared_ptr<WorldInformation>& initial_world_information) {
	//init View
	unsigned int seed;
	if(params.find("VIEW_SEED") != params.end()) {
		try {
			seed = boost::lexical_cast<unsigned int> (params["VIEW_SEED"]);
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading seed for view.");
		}
	} else {
		seed = 123;
	}

	boost::shared_ptr<DistributionGenerator> generator(new DistributionGenerator(seed));
	View::set_distribution_generator(generator);


	std::string robot_type = params["ROBOT_CONTROL"];
	boost::shared_ptr<RobotControl> control;

	if(robot_type == "UNIFORM_ROBOT_CONTROL") {
		control.reset(new UniformRobotControl(view_factory_factory(params), history_length, initial_world_information));
	} else if(robot_type == "ROBOT_TYPE_ROBOT_CONTROL") {
		boost::array<boost::shared_ptr<AbstractViewFactory>,kRobotTypeCount> view_factories;
		view_factories[MASTER] = view_factory_factory(params, "MASTER_");
		view_factories[SLAVE] = view_factory_factory(params, "SLAVE_");

		control.reset(new RobotTypeRobotControl(view_factories, history_length, initial_world_information));
	} else {
		throw UnsupportedOperationException("No RobotControl specified!");
	}

	return control;
}

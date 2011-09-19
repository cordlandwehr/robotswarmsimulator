/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <boost/program_options.hpp>

#include <iostream>

#include "../ActivationSequenceGenerators/activation_sequence_generator.h"
#include "../ActivationSequenceGenerators/synchronous_asg.h"
#include "../ActivationSequenceGenerators/synchronous_asg_wm.h"
#include "../ActivationSequenceGenerators/fair_atomic_semisynchronous_asg.h"
#include "../ActivationSequenceGenerators/atomic_semisynchronous_asg.h"
#include "../ActivationSequenceGenerators/asynchronous_asg.h"

#include "../Views/abstract_view_factory.h"
#include "../Views/view_factory.h"
#include "../Views/parametrized_view_factory.h"
#include "../ViewModels/global_view.h"
#include "../ViewModels/self_view.h"
#include "../ViewModels/local_graph_view.h"

#include "../EventHandlers/event_handler.h"
#include "../EventHandlers/vector_request_handler.h"

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

#include "../SimulationKernel/robot_control.h"
#include "../SimulationKernel/uniform_robot_control.h"
#include "../Model/world_information.h"
#include "../Model/robot_data.h"
#include "../WorldModifierImplementations/test_world_modifier.h"
#include "../WorldModifierImplementations/lua_world_modifier.h"
#include "../WorldModifierImplementations/circle_layouter.h"
#include "../WorldModifierImplementations/random_layouter.h"
#include "../WorldModifierImplementations/force_directed_layouter.h"
#include "../WorldModifierImplementations/default_world_modifier.h"

boost::filesystem::path Factory::project_directory_path;

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


boost::shared_ptr<EventHandler> Factory::event_handler_factory(boost::shared_ptr<History> history,
                                                               boost::shared_ptr<RobotControl> robot_control) {
	boost::shared_ptr<EventHandler> event_handler(new EventHandler(history, robot_control));

	// create the request handlers from the parameters


	// 3. Position Request Handler
	//std::string position_request_handler_type = boost::any_cast<std::string> (params["POSITION_REQUEST_HANDLER_TYPE"]);
	//if(position_request_handler_type == "VECTOR") {
		//try {
			// build the vector request handler
			double discard_probability = 0; // boost::lexical_cast<double> (params["VECTOR_POSITION_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = 0;// boost::lexical_cast<unsigned int> (params["VECTOR_POSITION_REQUEST_HANDLER_SEED"]);
			boost::shared_ptr<VectorRequestHandler> vector_request_handler(new VectorRequestHandler(seed, discard_probability, *history));
			boost::shared_ptr<InsertEdgeRequestHandler> insert_edge_request_handler(new InsertEdgeRequestHandler(seed, discard_probability, *history));
			boost::shared_ptr<MarkerRequestHandler> marker_request_handler(new MarkerRequestHandler(seed, discard_probability, *history));
			boost::shared_ptr<RemoveEdgeRequestHandler> remove_edge_request_handler(new RemoveEdgeRequestHandler(seed, discard_probability, *history));
			boost::shared_ptr<RemoveMessageRequestHandler> remove_message_request_handler(new RemoveMessageRequestHandler(seed, discard_probability, *history));
			boost::shared_ptr<SendMessageRequestHandler> send_message_request_handler(new SendMessageRequestHandler(seed, discard_probability, *history));

			/*
			// set up vector modifiers
			create_vector_modifiers_from_string(vector_request_handler,
					                            params["VECTOR_POSITION_REQUEST_HANDLER_MODIFIER"]);
			*/
			event_handler->set_position_request_handler(vector_request_handler);
			event_handler->set_insert_edge_request_handler(insert_edge_request_handler);
			event_handler->set_marker_request_handler(marker_request_handler);
			event_handler->set_remove_edge_request_handler(remove_edge_request_handler);
			event_handler->set_remove_message_request_handler(remove_message_request_handler);
			event_handler->set_send_message_request_handler(send_message_request_handler);
		/*} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for vector position request handler");
		}*/
	//}


	return event_handler;
}

boost::shared_ptr<ActivationSequenceGenerator> Factory::asg_factory(boost::program_options::variables_map &params, boost::shared_ptr<WorldInformation> initial_world_information) {

	std::string asg_type = params["ASG"].as<std::string>();
	boost::shared_ptr<ActivationSequenceGenerator> asg;
	// setup of activation sequence generator
	if(asg_type == "SYNCHRONOUS") {
	  throw UnsupportedOperationException("SYNCHRONOUS ASG is no longer supported. Use SYNCHRONOUS_WM instead.");
        //asg.reset(new SynchronousASG());
	} else if(asg_type == "SYNCHRONOUS_WM") {
        // TODO: Load/read actual world modifiers?
        asg.reset(new SynchronousASGWM(initial_world_information));
	} else if(asg_type == "ATOMIC_SEMISYNCHRONOUS") {
		throw UnsupportedOperationException("ATOMIC_SEMISYNCHRONOUS ASG is currently not supported.");
		
		/*try {
			unsigned int seed = params["ATOMIC_SEMISYNC_ASG_SEED"].as<unsigned int>();
			asg.reset(new AtomicSemisynchronousASG(seed));
		} catch(const boost::bad_lexical_cast&) {
			throw UnsupportedOperationException("Failed reading parameters for atomic semisynchronous asg.");
		}*/
	} else if(asg_type == "FAIR_ATOMIC_SEMISYNCHRONOUS") {
		throw UnsupportedOperationException("FAIR_ATOMIC_SEMISYNCHRONOUS ASG is currently not supported.");
		/*try {
			unsigned int seed = params["FAIR_ATOMIC_SEMISYNC_ASG_SEED"].as<unsigned int>();
			asg.reset(new FairAtomicSemisynchronousASG(seed));
		} catch(const boost::bad_lexical_cast&) {
			throw UnsupportedOperationException("Failed reading parameters for fair atomic semisynchronous asg.");
		}*/
	} else if(asg_type == "ASYNCHRONOUS") {
		throw UnsupportedOperationException("ASYNCHRONOUS ASG is currently not supported.");
		/*try {
			unsigned int seed = params["ASYNC_ASG_SEED"].as<unsigned int>();
			double participation_probability = params["ASYNC_ASG_PART_P"].as<double>();
			double p = params["ASYNC_ASG_TIME_P"].as<double>();
			asg.reset(new AsynchronousASG(seed, participation_probability, p));
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading parameters for asynchronous asg.");
		}*/
	} else {
		throw UnsupportedOperationException("No ASG specified!");
	}

	return asg;
}

boost::shared_ptr<AbstractViewFactory> Factory::view_factory_factory(boost::program_options::variables_map &params, const std::string& prefix) {
	std::string view_type;
  
	if (params.count(prefix + "VIEW")) {
		try {
		  view_type = params[prefix + "VIEW"].as<std::string>();
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading view parameter.");
		}	  
	} else {
	  ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::info) << "No View specified! Defaulting to local graph view.";
	  view_type = "LOCAL_GRAPH_VIEW"; //default value
	}
	  
	view_type.erase(0,prefix.length()); //erase prefix
	boost::shared_ptr<AbstractViewFactory> view_factory;

	if(view_type == "GLOBAL_VIEW") {
		view_factory.reset(new ViewFactory<GlobalView>);
	} else if(view_type == "LOCAL_GRAPH_VIEW"){
		view_factory.reset(new ViewFactory<LocalGraphView>);
	} else {
		ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::info) << "No View specified! Defaulting to local graph view.";
		view_factory.reset(new ViewFactory<LocalGraphView>);
	}

	return view_factory;
}

boost::shared_ptr<Robot> Factory::robot_factory(boost::shared_ptr<RobotIdentifier> id, const std::string& algorithm, bool full_path) {

	boost::shared_ptr<Robot> robot;

	// check if the algorithm is a lua file
	std::string subfix;
	if(algorithm.size() >= 4)
		subfix = algorithm.substr(algorithm.size() - 4, 4);
	if(subfix == ".lua") {
		if(full_path )
			robot.reset(new LuaRobot(id, algorithm));
		else
			robot.reset(new LuaRobot(id, (project_directory_path / algorithm).string()));

	} else if(algorithm == "SimpleRobot" || algorithm == "NONE") {
		robot.reset(new SimpleRobot(id));
	} else {
		throw UnsupportedOperationException("Tried to create unknown robot type: "+algorithm);
	}
	return robot;
}

boost::shared_ptr<WorldModifier> Factory::world_modifier_factory(const std::string &str) {
	boost::shared_ptr<WorldModifier> world_modifier;

	// check if the algorithm is a lua file
	std::string subfix;
	if(str.size() >= 4)
		subfix = str.substr(str.size() - 4, 4);

	if(str=="NONE"){
		world_modifier.reset(new DefaultWorldModifier());
	}
	else if(subfix == ".lua") {
		world_modifier.reset(new LuaWorldModifier(str));
	}
	else if(str == "TestWorldModifier") {
		world_modifier.reset(new TestWorldModifier());
	}
	else if(str == "CircleLayouter"){
		world_modifier.reset(new CircleLayouter());
	}
	else if(str == "RandomLayouter"){
		world_modifier.reset(new RandomLayouter());
	}
	else if(str == "ForceDirectedLayouter"){
		world_modifier.reset(new ForceDirectedLayouter());
	}
	else {
		throw UnsupportedOperationException("Tried to create unknown world modifier type: " + str);
	}

	return world_modifier;
}

boost::shared_ptr<RobotControl> Factory::robot_control_factory(boost::program_options::variables_map params, std::size_t history_length, const boost::shared_ptr<WorldInformation>& initial_world_information) {
	//TODO asetzer change it so that not params, but the actual seed is given as a parameter
  
  //init View
	unsigned int seed;
	if(params.count("VIEW_SEED")) {
		try {
			seed = params["VIEW_SEED"].as<unsigned int>();
		} catch(const boost::bad_lexical_cast& ) {
			throw UnsupportedOperationException("Failed reading seed for view.");
		}
	} else {
		seed = 123;
	}

	boost::shared_ptr<DistributionGenerator> generator(new DistributionGenerator(seed));
	View::set_distribution_generator(generator);
	
	boost::shared_ptr<RobotControl> control;
	
	control.reset(new UniformRobotControl(view_factory_factory(params), history_length, initial_world_information));

	return control;
}

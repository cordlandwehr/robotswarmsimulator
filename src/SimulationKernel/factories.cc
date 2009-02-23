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


#include "../ActivationSequenceGenerators/activation_sequence_generator.h"
#include "../ActivationSequenceGenerators/synchronous_asg.h"
#include "../ActivationSequenceGenerators/asynchronous_asg.h"

#include "../Views/abstract_view_factory.h"
#include "../Views/view_factory.h"
#include "../Views/parametrized_view_factory.h"
#include "../Views/global_view.h"
#include "../Views/cog_view.h"
#include "../Views/one_point_formation_view.h"
#include "../Views/chain_view.h"

#include "../EventHandlers/event_handler.h"
#include "../EventHandlers/marker_request_handler.h"
#include "../EventHandlers/type_change_request_handler.h"
#include "../EventHandlers/vector_request_handler.h"

#include "../Utilities/VectorModifiers/vector_modifier.h"
#include "../Utilities/VectorModifiers/vector_difference_trimmer.h"
#include "../Utilities/VectorModifiers/vector_trimmer.h"
#include "../Utilities/VectorModifiers/vector_randomizer.h"

#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../RobotImplementations/lua_robot.h"
#include "../RobotImplementations/simple_robot.h"

#include <iostream>

void set_up_vector_modifiers(boost::shared_ptr<VectorRequestHandler> handler,
                             std::vector<boost::tuple<std::string, std::vector<boost::any> > > & modifiers ) {


	std::vector<boost::tuple<std::string, std::vector<boost::any> > >::iterator modifier_iter = modifiers.begin();
	while(modifier_iter != modifiers.end()) {
		std::string type = (*modifier_iter).get<0>();
		boost::shared_ptr<VectorModifier> vector_modifier;
		if(type == "VECTOR_TRIMMER") {
			double length = boost::any_cast<double>((*modifier_iter).get<1>().at(0));
			vector_modifier.reset(new VectorTrimmer(length));
		} else if(type == "VECTOR_DIFFERENCE_TRIMMER") {
			double length = boost::any_cast<double>((*modifier_iter).get<1>().at(0));
			vector_modifier.reset(new VectorDifferenceTrimmer(length));
		} else if(type == "VECTOR_RANDOMIZER") {
			unsigned int seed = boost::any_cast<unsigned int>((*modifier_iter).get<1>().at(0));
			double standard_deviation = boost::any_cast<double>((*modifier_iter).get<1>().at(1));
			vector_modifier.reset(new VectorRandomizer(seed, standard_deviation));
		} else {
			//TODO(craupach) throw up
		}
		handler->add_vector_modifier(vector_modifier);
		modifier_iter++;
	}

}




boost::shared_ptr<EventHandler> Factory::event_handler_factory(std::map<std::string, boost::any> &params,
                                                               boost::shared_ptr<History> history,
                                                               boost::shared_ptr<RobotControl> robot_control) {
	boost::shared_ptr<EventHandler> event_handler(new EventHandler(history, robot_control));

	// create the request handlers from the parameters

	// 1. Marker Request Handler
	std::string marker_request_handler_type = boost::any_cast<std::string> (params["MARKER_REQUEST_HANDLER_TYPE"]);
	if(marker_request_handler_type == "STANDARD") {
			// build the marker request handler
			double discard_probability = boost::any_cast<double> (params["STANDARD_MARKER_REQUEST_HANDLER_DISCARD_PROB"]);
			unsigned int seed = boost::any_cast<unsigned int> (params["STANDARD_MARKER_REQUEST_HANDLER_SEED"]);
			boost::shared_ptr<MarkerRequestHandler> marker_request_handler(new MarkerRequestHandler(seed, discard_probability, *history));
			event_handler->set_marker_request_handler(marker_request_handler);
	}

	// 2. Type Change Request Handler
	std::string type_change_request_handler_type = boost::any_cast<std::string> (params["TYPE_CHANGE_REQUEST_HANDLER_TYPE"]);
	if(type_change_request_handler_type == "STANDARD") {
		// build the marker request handler
		double discard_probability = boost::any_cast<double> (params["STANDARD_TYPE_CHANGE_REQUEST_HANDLER_DISCARD_PROB"]);
		unsigned int seed = boost::any_cast<unsigned int> (params["STANDARD_TYPE_CHANGE_REQUEST_HANDLER_SEED"]);
		boost::shared_ptr<TypeChangeRequestHandler> type_change_request_handler(new TypeChangeRequestHandler(seed, discard_probability, *history));
		event_handler->set_type_change_request_handler(type_change_request_handler);
	}

	// 3. Position Request Handler
	std::string position_request_handler_type = boost::any_cast<std::string> (params["POSITION_REQUEST_HANDLER_TYPE"]);
	if(position_request_handler_type == "VECTOR") {
		// build the marker request handler
		double discard_probability = boost::any_cast<double> (params["VECTOR_POSITION_REQUEST_HANDLER_DISCARD_PROB"]);
		unsigned int seed = boost::any_cast<unsigned int> (params["VECTOR_POSITION_REQUEST_HANDLER_SEED"]);
		boost::shared_ptr<VectorRequestHandler> vector_request_handler(new VectorRequestHandler(seed, discard_probability, *history));

		// set up vector modifiers
		std::vector<boost::tuple<std::string, std::vector<boost::any> > > modifiers =
		    boost::any_cast< std::vector<boost::tuple<std::string,std::vector<boost::any> > > > (params["VECTOR_POSITION_REQUEST_HANDLER_MODIFIER"]);
		set_up_vector_modifiers(vector_request_handler, modifiers);

		event_handler->set_position_request_handler(vector_request_handler);

	}

	// 4. Velocity Request Handler
	std::string velocity_request_handler_type = boost::any_cast<std::string> (params["VELOCITY_REQUEST_HANDLER_TYPE"]);
	if(velocity_request_handler_type == "VECTOR") {
		// build the marker request handler
		double discard_probability = boost::any_cast<double> (params["VECTOR_VELOCITY_REQUEST_HANDLER_DISCARD_PROB"]);
		unsigned int seed = boost::any_cast<unsigned int> (params["VECTOR_VELOCITY_REQUEST_HANDLER_SEED"]);
		boost::shared_ptr<VectorRequestHandler> vector_request_handler(new VectorRequestHandler(seed, discard_probability, *history));

		// set up vector modifiers
		std::vector<boost::tuple<std::string, std::vector<boost::any> > > modifiers =
		    boost::any_cast< std::vector<boost::tuple<std::string,std::vector<boost::any> > > > (params["VECTOR_VELOCITY_REQUEST_HANDLER_MODIFIER"]);
		set_up_vector_modifiers(vector_request_handler, modifiers);

		event_handler->set_velocity_request_handler(vector_request_handler);
	}

	// 5. Acceleration Request Handler
	std::string acceleration_request_handler_type = boost::any_cast<std::string> (params["ACCELERATION_REQUEST_HANDLER_TYPE"]);
	if(acceleration_request_handler_type == "VECTOR") {
		// build the marker request handler
		double discard_probability = boost::any_cast<double> (params["VECTOR_ACCELERATION_REQUEST_HANDLER_DISCARD_PROB"]);
		unsigned int seed = boost::any_cast<unsigned int> (params["VECTOR_ACCELERATION_REQUEST_HANDLER_SEED"]);
		boost::shared_ptr<VectorRequestHandler> vector_request_handler(new VectorRequestHandler(seed, discard_probability, *history));

		// set up vector modifiers
		std::vector<boost::tuple<std::string, std::vector<boost::any> > > modifiers =
		    boost::any_cast< std::vector<boost::tuple<std::string,std::vector<boost::any> > > > (params["VECTOR_ACCELERATION_REQUEST_HANDLER_MODIFIER"]);
		set_up_vector_modifiers(vector_request_handler, modifiers);

		event_handler->set_acceleration_request_handler(vector_request_handler);
	}

	return event_handler;
}

boost::shared_ptr<ActivationSequenceGenerator> Factory::asg_factory(std::map<std::string, boost::any> &params) {

	std::string asg_type = boost::any_cast<std::string> (params["ASG"]);
	boost::shared_ptr<ActivationSequenceGenerator> asg;
	// setup of activation sequence generator
	if(asg_type == "SYNCHRONOUS") {
			asg.reset(new SynchronousASG());
	} else if(asg_type == "SEMISYNCHRONOUS") {
			//TODO(craupach) make something semi-synchrounous here;
	} else if(asg_type == "ASYNCHRONOUS") {
			unsigned int seed = boost::any_cast<unsigned int>(params["ASYNC_ASG_SEED"]);
			double participation_probability = boost::any_cast<double>(params["ASYNC_ASG_PART_P"]);
			double p = boost::any_cast<double>(params["ASYNC_ASG_TIME_P"]);
			asg.reset(new AsynchronousASG(seed, participation_probability, p));
	}

	return asg;
}

boost::shared_ptr<AbstractViewFactory> Factory::view_factory_factory(std::map<std::string, boost::any> &params) {

	std::string view_type = boost::any_cast<std::string> (params["VIEW"]);
	boost::shared_ptr<AbstractViewFactory> view_factory;

	if(view_type == "GLOBAL_VIEW") {
		view_factory.reset(new ViewFactory<GlobalView>);
	} else if(view_type == "COG_VIEW") {
		view_factory.reset(new ViewFactory<CogView>);
	} else if(view_type == "CHAIN_VIEW") {
		unsigned int number = boost::any_cast<unsigned int>(params["CHAIN_VIEW_NUM_ROBOTS"]);
		view_factory.reset(new ParametrizedViewFactory<ChainView, unsigned int>(number));
	} else if(view_type == "ONE_POINT_FORMATION_VIEW") {
		double radius = boost::any_cast<double>(params["ONE_POINT_FORMATION_VIEW_RADIUS"]);
		view_factory.reset(new ParametrizedViewFactory<OnePointFormationView, double>(radius));
	} else if(view_type == "NONE") {
		view_factory.reset(new ViewFactory<View>);
	}

	return view_factory;
}

boost::shared_ptr<Robot> Factory::robot_factory(boost::shared_ptr<RobotIdentifier> id, const std::string& algorithm) {

	boost::shared_ptr<Robot> robot;

	// check if the algorithm is a lua file
	std::string subfix = algorithm.substr(algorithm.size() - 4, 4);
	if(subfix == ".lua") {
		robot.reset(new LuaRobot(id, algorithm));
	} else if(algorithm == "SIMPLEROBOT") {
		robot.reset(new SimpleRobot(id));
	}
	return robot;
}

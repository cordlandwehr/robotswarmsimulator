/*
 * factories.cc
 *
 *  Created on: Feb 22, 2009
 *      Author: craupach
 */

#include "factories.h"
#include <string>
#include <boost/any.hpp>

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
		boost::shared_ptr<MarkerRequestHandler> marker_request_handler(new MarkerRequestHandler(seed, discard_probability, *history));
		event_handler->set_marker_request_handler(marker_request_handler);
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

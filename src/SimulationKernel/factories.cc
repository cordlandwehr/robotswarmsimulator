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
#include "../Views/global_view.h"

boost::shared_ptr<EventHandler> Factory::event_handler_factory(std::map<std::string, boost::any> &params) {

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

	if(view_type == "GLOBALVIEW") {
		view_factory.reset(new ViewFactory<GlobalView>);
	} else if(view_type == "FULLVIEW") {
		view_factory.reset(new ViewFactory<FullView>);
	}

	return view_factory;
}

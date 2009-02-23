/*
 * factories.h
 *
 *  Created on: Feb 22, 2009
 *      Author: craupach
 */

#ifndef FACTORIES_H_
#define FACTORIES_H_

#include <map>
#include <string>
#include <boost/any.hpp>
#include <boost/smart_ptr.hpp>

class EventHandler;
class ActivationSequenceGenerator;
class AbstractViewFactory;

namespace Factory {

	boost::shared_ptr<EventHandler> event_handler_factory(std::map<std::string, boost::any> &params);
	boost::shared_ptr<ActivationSequenceGenerator> asg_factory(std::map<std::string, boost::any> &params);
	boost::shared_ptr<AbstractViewFactory> view_factory_factory(std::map<std::string, boost::any> &params);
}

#endif /* FACTORIES_H_ */

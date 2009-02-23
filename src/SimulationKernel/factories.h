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
class History;
class RobotControl;
class Robot;
class RobotIdentifier;

namespace Factory {

	boost::shared_ptr<EventHandler> event_handler_factory(std::map<std::string, boost::any> &params,
	                                                      boost::shared_ptr<History> history,
														  boost::shared_ptr<RobotControl> robot_control);
	boost::shared_ptr<ActivationSequenceGenerator> asg_factory(std::map<std::string, boost::any> &params);
	boost::shared_ptr<AbstractViewFactory> view_factory_factory(std::map<std::string, boost::any> &params);
	boost::shared_ptr<Robot> robot_factory(boost::shared_ptr<RobotIdentifier> id, const std::string &algorithm);
}

#endif /* FACTORIES_H_ */
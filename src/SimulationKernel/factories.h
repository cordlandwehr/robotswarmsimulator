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
#include <cstddef>
#include <boost/any.hpp>
#include <boost/smart_ptr.hpp>

class EventHandler;
class ActivationSequenceGenerator;
class AbstractViewFactory;
class History;
class RobotControl;
class Robot;
class RobotIdentifier;
class WorldInformation;

/**
 * This namespace provides factory functions for the simulator components.
 */
namespace Factory {
	// TODO(craupach) test test test...

	/**
	 * Creates event handler from parameters given in the params map.
	 * Returns shared pointer to created event handler
	 * See UserGuide for expected parameters
	 */
	boost::shared_ptr<EventHandler> event_handler_factory(std::map<std::string, std::string> &params,
	                                                      boost::shared_ptr<History> history,
														  boost::shared_ptr<RobotControl> robot_control);

	/**
	 * Creates activation sequence generator from parameters given in the params map
	 * Returns shared pointer to created activation sequence generator
	 * See UserGuide for expected parameters
	 */
	boost::shared_ptr<ActivationSequenceGenerator> asg_factory(std::map<std::string, std::string> &params);

	/**
	 * Creates ViewFactory from parameters given in the param map.
	 * Returns shared pointer to ViewFactory
	 * See UserGuide for expected parameters
	 */
	boost::shared_ptr<AbstractViewFactory> view_factory_factory(std::map<std::string, std::string> &params, const std::string& prefix = "");

	/**
	 * Creates RobotControl from parameters given in the param map.
	 * Returns shared pointer to RobotControl
	 * See UserGuide for expected parameters
	 */
	boost::shared_ptr<RobotControl> robot_control_factory(std::map<std::string, std::string> &params, std::size_t history_length, const WorldInformation& initial_world_information);

	/**
	 * Creates robot from given algorithm
	 * Algorithm may be a class or a lua-file.
	 */
	boost::shared_ptr<Robot> robot_factory(boost::shared_ptr<RobotIdentifier> id, const std::string &algorithm);
}

#endif /* FACTORIES_H_ */

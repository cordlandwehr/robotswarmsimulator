/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
#include <boost/program_options/variables_map.hpp>

#include "../Model/world_modifier.h"

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
	boost::shared_ptr<ActivationSequenceGenerator> asg_factory(boost::program_options::variables_map &params);

	/**
	 * Creates ViewFactory from parameters given in the param map.
	 * Returns shared pointer to ViewFactory
	 * See UserGuide for expected parameters
	 */
	boost::shared_ptr<AbstractViewFactory> view_factory_factory(boost::program_options::variables_map &params, const std::string& prefix = "");

	/**
	 * Creates RobotControl from parameters given in the param map.
	 * Returns shared pointer to RobotControl
	 * See UserGuide for expected parameters
	 */
	boost::shared_ptr<RobotControl> robot_control_factory(boost::program_options::variables_map params, std::size_t history_length, const boost::shared_ptr<WorldInformation>& initial_world_information);

	/**
	 * Creates robot from given algorithm
	 * Algorithm may be a class or a lua-file.
	 */
	boost::shared_ptr<Robot> robot_factory(boost::shared_ptr<RobotIdentifier> id, const std::string &algorithm);

	/**
	 * Creates world modifier from given string
	 * String may be a class or a lua-file.
	 */
	boost::shared_ptr<WorldModifier> world_modifier_factory(const std::string &str);
}

#endif /* FACTORIES_H_ */

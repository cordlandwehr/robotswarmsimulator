/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

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
 * request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */
#include "../EventHandlers/request_handler.h"

#include <boost/shared_ptr.hpp>

#include "../SimulationControl/history.h"
#include "../Model/world_information.h"
#include "../Requests/request.h"
#include "../Utilities/distribution_generator.h"

RequestHandler::RequestHandler(
    unsigned int seed,
    double discard_probability,
    const History& history) : history_(history),
                              discard_probability_(discard_probability),
                              distribution_generator_(
                                  new DistributionGenerator(seed)) {
	distribution_generator_->init_bernoulli(discard_probability_);
}

RequestHandler::~RequestHandler() {}

bool RequestHandler::handle_request(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {
	if (!distribution_generator_->get_value_bernoulli()) {
		return handle_request_reliable(world_information, request);
	}
	return false;
}

/*
 * request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */
#include <boost/shared_ptr.hpp>

#include "../SimulationControl/history.h"
#include "../Model/world_information.h"
#include "../Requests/request.h"
#include "../Utilities/distribution_generator.h"

#include "request_handler.h"

RequestHandler::RequestHandler(unsigned int seed,
                               double discard_probability,
                               const History& history) : history_(history),
                                                         discard_probability_(discard_probability),
                                                         distribution_generator_(new DistributionGenerator(seed)) {
	distribution_generator_->init_bernoulli(discard_probability_);
}

RequestHandler::~RequestHandler() {};

bool RequestHandler::handle_request(boost::shared_ptr<WorldInformation> world_information,
                                    boost::shared_ptr<const Request> request) {
	if (!distribution_generator_->get_value_bernoulli()) {
		return handle_request_reliable(world_information, request);
	}
	return false;
}

/*
 * vector_request_handler.cc
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/request.h"

#include "vector_request_handler.h"

 void VectorRequestHandler::handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                                    boost::shared_ptr<const Request> request) {
	 // TODO(craupach) implement
 }

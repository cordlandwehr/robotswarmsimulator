/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 *  test_type_change_request_handler.h
 *
 *  Created on: Dec 29, 2008
 *      Author: peter
 */

#ifndef TEST_TYPE_CHANGE_REQUEST_HANDLER_H_
#define TEST_TYPE_CHANGE_REQUEST_HANDLER_H_

#include "../../EventHandlers/type_change_request_handler.h"


/**
 * \brief Simple type_change request handler that counts the number of (not discarded) requests handled.
 *
 * Apart from the counting, this request handler behaves exactly as the standard type_change request handler.
 */
class TestTypeChangeRequestHandler : public TypeChangeRequestHandler {
public:
	TestTypeChangeRequestHandler(unsigned int seed, double discard_probability, const History& history)
	: TypeChangeRequestHandler(seed, discard_probability, history), nr_handled_(0) {
	}
	
	unsigned int nr_handled() { return nr_handled_; }
	
protected:
	bool handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
								 boost::shared_ptr<const Request> request) {
		nr_handled_++;
		return TypeChangeRequestHandler::handle_request_reliable(world_information, request);
	}
	
private:
	unsigned int nr_handled_;
};


#endif /* TEST_TYPE_CHANGE_REQUEST_HANDLER_H_ */

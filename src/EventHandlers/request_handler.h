/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>

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
 * request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef REQUEST_HANDLER_H_
#define REQUEST_HANDLER_H_

#include <boost/shared_ptr.hpp>
#include "../SimulationControl/history.h"

class History;
class DistributionGenerator;
class Request;
class WorldInformation;

/**
 * abstract base class for handling requests. It implements unreliable handling of requests by
 * randomly discarding them with the given discard probability.
 */
class RequestHandler {
	friend class EventHandlerFactoryTest;
	friend class LoadMainProjectFileTest;
public:
	/**
	 * constructs the RequestHandler by setting up the distribution generator with the given
	 * seed and discard probability.
	 */
	RequestHandler(unsigned int seed, double discard_probability,
	               const History& history);
	virtual ~RequestHandler() = 0;

	/**
	 * discards the given request with the given probability. If not discarded the request is
	 * delegated to the abstract handle_request_reliable member.
	 *
	 * Non-abstract RequestHandlers should implement handle_request_reliable
	 *
	 * \return Returns true if event was handled without any change, false otherwise (e.g. if it was decarded or not
	 *         performed completely.
	 */
	bool handle_request(boost::shared_ptr<WorldInformation> world_information,
	                    boost::shared_ptr<const Request> request);

protected:
	/**
	 * History may be used by inheriting classes to implement adversial behavior.
	 */
	const History& history_;

private:
	/**
	 * \return Subclasses have to use the return value to indicate wether
	 *         they changed the incoming request in any way before it was
	 *         performed  (if it has been performed at all).
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request) = 0;

	double discard_probability_;
	boost::shared_ptr<DistributionGenerator> distribution_generator_;
};

#endif /* REQUEST_HANDLER_H_ */

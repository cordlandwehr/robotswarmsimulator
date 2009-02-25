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
	RequestHandler(unsigned int seed, double discard_probability, const History& history);
	virtual ~RequestHandler() = 0;

	/**
	 * discards the given request with the given probability. If not discarded the request is
	 * delegated to the abstract handle_request_reliable member.
	 *
	 * Non-abstract RequestHandlers should implement handle_request_reliable
	 */
	void handle_request(boost::shared_ptr<WorldInformation> world_information,
	                    boost::shared_ptr<const Request> request);

protected:
	/**
	 * History may be used by inheriting classes to implement adversial behavior.
	 */
	const History& history_;

private:
	virtual void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
	                                     boost::shared_ptr<const Request> request) = 0;

	double discard_probability_;
	boost::shared_ptr<DistributionGenerator> distribution_generator_;
};

#endif /* REQUEST_HANDLER_H_ */

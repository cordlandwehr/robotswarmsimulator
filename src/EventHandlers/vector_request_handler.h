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
 * vector_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef VECTOR_REQUEST_HANDLER_H_
#define VECTOR_REQUEST_HANDLER_H_

#include "request_handler.h"

#include <list>

#include <boost/shared_ptr.hpp>

#include "../Utilities/vector_arithmetics.h"

class VectorModifier;
class VectorRequest;
class RobotData;


/**
 * \brief Handles vector requests (e.g. position, velocity and acceleration requests).
 *
 * To configure a vector request handler, you can add so called vector modifiers. Given a request to be handled, the
 * vector request handler extracts the corresponding vector and applies all initially provided vector modifiers to it
 * (in the same order they were added to the vector request handler). The resulting vector is used to build a new,
 * modified request which is afterwards handled normally.
 *
 * If there is need for more sophisticated request handling, one may still subclass VectorRequestHandler and overwrite
 * the 'handle_request_reliable' method.
 */
class VectorRequestHandler : public RequestHandler {
	friend class LoadMainProjectFileTest;
public:
	VectorRequestHandler(unsigned int seed,
	                     double discard_probability,
	                     const History& history)
	: RequestHandler(seed, discard_probability, history) { }
    virtual ~VectorRequestHandler() { }

	/**
	 * \brief Adds the given vector modifier to the end of the request handlers modifier pipeline.
	 *
	 * Note that the order in which modifiers are added is important.
	 */
	void add_vector_modifier(boost::shared_ptr<VectorModifier> vector_modifier);

protected:
	/**
	 * \brief Handles a given vector request by extracting the
	 * corresponding vector and applying the modifier pipeline.
	 *
	 * For each vector modifier applied, the reference vector is set to
	 * the original value of the data the robot
	 * requested to change.
	 *
	 * \return Returns true if the request has been performed as given
	 * and false if it was changed (by one of the vector
	 *         modifiers).
	 * \see extract_ref_vector
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);

private:
	/**
	 * \brief Updates the robot property corresponding to
	 * the given vector request using the given vector.
	 */
    void update_vector(const VectorRequest& vector_request,
                       RobotData& robot_data,
                       const Vector3d& vector);

	/**
	 * \brief Extracts the reference vector for the given request from the robot data.
	 *
	 * The reference vector matches the robot's original value of the data
	 * he requests to change. For example, in case of a positional request,
	 * the robot's current position will be extracted.
	 */
	static const Vector3d& extract_ref_vector(const VectorRequest& request,
	                                          const RobotData& robot_data);

	std::list<boost::shared_ptr<VectorModifier> > vector_modifiers_;
};

#endif /* VECTOR_REQUEST_HANDLER_H_ */

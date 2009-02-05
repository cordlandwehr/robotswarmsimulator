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

#include "vector_modifier.h"
#include "../Utilities/vector3d.h"

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
public:
	VectorRequestHandler(unsigned int seed,
                         double discard_probability,
                         const History& history) : RequestHandler(seed, discard_probability, history) {}
    virtual ~VectorRequestHandler() {}
	
	/**
	 * \brief Adds the given vector modifier to the request handlers modifier pipeline.
	 *
	 * Note that the order in which modifiers are added is important.
	 */
	void add_vector_modifier(boost::shared_ptr<VectorModifier> vector_modifier);
	
protected:
	/**
	 * \brief Handles a given vector request by extracting the corresponding vector and applying the modifier pipeline.
	 */
	virtual void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                         boost::shared_ptr<const Request> request);
	
private:
    void apply_request(boost::shared_ptr<const VectorRequest> vector_request,
    		           RobotData & robot_data,
    		           boost::shared_ptr<Vector3d> processed_global_vector);
	
	std::list<boost::shared_ptr<VectorModifier> > vector_modifiers_;
};

#endif /* VECTOR_REQUEST_HANDLER_H_ */

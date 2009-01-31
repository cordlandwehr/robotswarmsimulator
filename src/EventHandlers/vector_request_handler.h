/*
 * vector_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef VECTOR_REQUEST_HANDLER_H_
#define VECTOR_REQUEST_HANDLER_H_

#include "request_handler.h"
#include "../Utilities/vector3d.h"

class VectorRequest;
class RobotData;

class VectorRequestHandler : public RequestHandler {
public:
	VectorRequestHandler(unsigned int seed,
                         double discard_probability,
                         const History& history) : RequestHandler(seed, discard_probability, history) {}
    virtual ~VectorRequestHandler() {}
private:
	virtual void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                         boost::shared_ptr<const Request> request);
    void apply_request(boost::shared_ptr<const VectorRequest> vector_request,
    		           RobotData & robot_data,
    		           boost::shared_ptr<Vector3d> requested_global_vector);
};

#endif /* VECTOR_REQUEST_HANDLER_H_ */

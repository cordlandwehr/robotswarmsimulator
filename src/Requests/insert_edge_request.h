/*
 * message_request.h
 *
 *  Created on: 22.07.2011
 *      Author: Jonas
 */

#ifndef INSERT_EDGE_REQUEST_H_
#define INSERT_EDGE_REQUEST_H_

#include "request.h"
#include "../Model/edge.h"


class InsertEdgeRequest : public Request {
public:
		InsertEdgeRequest(Robot & robot, boost::shared_ptr<Edge> e) :
	    Request(robot),
	    requested_edge_(e) {}

	boost::shared_ptr<Edge> requested_edge() const { return requested_edge_; }

private:
	boost::shared_ptr<Edge> requested_edge_;
};


#endif /* INSERT_EDGE_REQUEST_H_ */

/*
 * remove_edge_request.h
 *
 *  Created on: 22.07.2011
 *      Author: Jonas
 */

#ifndef REMOVE_EDGE_REQUEST_H_
#define REMOVE_EDGE_REQUEST_H_

#include "request.h"
#include "../Model/edge.h"


class RemoveEdgeRequest : public Request {
public:
		RemoveEdgeRequest(Robot & robot, boost::shared_ptr<Edge> e) :
	    Request(robot),
	    requested_edge_(e) {}

	boost::shared_ptr<Edge> requested_edge() const { return requested_edge_; }

private:
	boost::shared_ptr<Edge> requested_edge_;
};


#endif /* REMOVE_EDGE_REQUEST_H_ */

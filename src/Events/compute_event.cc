/*
 * compute_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "compute_event.h"

void ComputeEvent::add_to_robot_subset(boost::shared_ptr<Robot> new_robot) {
	robot_subset_.push_back(new_robot);
}

//TODO(craupach): maybe this should be inline.
const vector<boost::shared_ptr<Robot> >& ComputeEvent::get_robot_subset() const {
	return robot_subset_;
}

void ComputeEvent::add_to_requests(boost::shared_ptr<Request> new_request) {
	requests_.push_back(new_request);
}

const vector<boost::shared_ptr<Request> >& ComputeEvent::get_requests() const {
	return requests_;
}

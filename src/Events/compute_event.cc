/*
 * compute_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "compute_event.h"

#include <list>
#include <boost/smart_ptr.hpp>

using boost::shared_ptr;

void ComputeEvent::add_to_robot_subset(shared_ptr<Robot> new_robot) {
	robot_subset_.push_back(new_robot);
}

const std::list<shared_ptr<Robot> >& ComputeEvent::robot_subset() const {
	return robot_subset_;
}

void ComputeEvent::add_to_requests(shared_ptr<const Request> new_request) {
	requests_.push_back(new_request);
}

const std::list<shared_ptr<const Request> >& ComputeEvent::requests() const {
	return requests_;
}

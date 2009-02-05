/*
 * handle_requests_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "handle_requests_event.h"

void HandleRequestsEvent::add_to_requests(boost::shared_ptr<const Request> new_request) {
	requests_.push_back(new_request);
}

const list<boost::shared_ptr<const Request> >& HandleRequestsEvent::requests() const {
	return requests_;
}

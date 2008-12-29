/*
 * handle_requests_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "handle_requests_event.h"

void HandleRequestsEvent::add_to_requests(boost::shared_ptr<Request> new_request) {
	requests_.push_back(new_request);
}

const vector<boost::shared_ptr<Request> >& HandleRequestsEvent::get_requests() const {
	return requests_;
}

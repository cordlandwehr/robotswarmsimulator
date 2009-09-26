/*
 * handle_requests_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "handle_requests_event.h"

using std::list;
using boost::shared_ptr;

void HandleRequestsEvent::add_to_requests(
    shared_ptr<const Request> new_request) {
	requests_.push_back(new_request);
}

const list<shared_ptr<const Request> >& HandleRequestsEvent::requests() const {
	return requests_;
}

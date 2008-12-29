/*
 * handle_request_event.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef HANDLE_REQUEST_EVENT_H_
#define HANDLE_REQUEST_EVENT_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include "event.h"
#include "../Model/robot.h"
#include "../Requests/request.h"

using namespace std;

/**
 * \class HandleRequestsEvent
 * \brief A HandleRequestsEvent is an event which causes a set of requests to be handled by the event handler
 *
 */
class HandleRequestsEvent : public Event {
public:

	HandleRequestsEvent(int time): Event(time) {};

	/**
	 * Adds a new request to the set of requests.
	 * \param A shared pointer to the new request.
	 */
	void add_to_requests(boost::shared_ptr<Request> new_request);

	/**
	 * Returns a constant reference to the set of requests.
	 * \return A constant reference to the set of requests.
	 */
	const vector<boost::shared_ptr<Request> >& get_requests() const;

private:
	/**
	 * The set of resulting requests
	 */
	vector<boost::shared_ptr<Request> > requests_;
};

#endif /* HANDLE_REQUEST_EVENT_H_ */
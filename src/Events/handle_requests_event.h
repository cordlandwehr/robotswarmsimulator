/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * handle_request_event.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef HANDLE_REQUEST_EVENT_H_
#define HANDLE_REQUEST_EVENT_H_

#include <list>
#include <boost/smart_ptr.hpp>
#include "event.h"
#include "../Model/robot.h"
#include "../Requests/request.h"


/**
 * \class HandleRequestsEvent
 * \brief A HandleRequestsEvent is an event which causes a set of requests to be handled by the event handler
 *
 */
class HandleRequestsEvent : public Event {
public:

	explicit HandleRequestsEvent(int time): Event(time) {}

	/**
	 * Adds a new request to the set of requests.
	 * \param A shared pointer to the new request.
	 */
	void add_to_requests(boost::shared_ptr<const Request> new_request);

	/**
	 * Returns a constant reference to the set of requests.
	 * \return A constant reference to the set of requests.
	 */
	const std::list<boost::shared_ptr<const Request> >& requests() const;

private:
	/**
	 * The set of resulting requests
	 */
	std::list<boost::shared_ptr<const Request> > requests_;
};

#endif /* HANDLE_REQUEST_EVENT_H_ */

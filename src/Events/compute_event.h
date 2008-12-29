/*
 * compute_event.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef COMPUTE_EVENT_H_
#define COMPUTE_EVENT_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include "event.h"
#include "../Model/robot.h"
#include "../Requests/request.h"

using namespace std;

/**
 * \class ComputeEvent
 * \brief A ComputeEvent is an event which causes a subset of the robots to calculate new requests
 *
 */
class ComputeEvent : public Event {
public:
	/**
	 * Adds a new robot to the subset of robots in the event.
	 * \param A shared pointer to the new robot.
	 */
	void add_to_robot_subset(boost::shared_ptr<Robot> new_robot);

	/**
	 * Returns a constant reference to the robot subset.
	 * \return A constant reference to the robot subset.
	 */
	const vector<boost::shared_ptr<Robot> >& get_robot_subset() const;

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
	 * The robot subset for this event.
	 */
	vector<boost::shared_ptr<Robot> > robot_subset_;

	/**
	 * The set of resulting requests
	 */
	vector<boost::shared_ptr<Request> > requests_;
};


#endif /* COMPUTE_EVENT_H_ */

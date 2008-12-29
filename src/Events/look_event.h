/*
 * look_event.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef LOOK_EVENT_H_
#define LOOK_EVENT_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include "event.h"
#include "../Model/robot.h"

using namespace std;

/**
 * \class LookEvent
 * \brief A LookEvent is an event which causes a subset of the robots to recieve new
 *        information about the world
 *
 */
class LookEvent : public Event {
public:

	LookEvent(int time) : Event(time) {};

	/**
	 * Adds a new robot to the subset of robots in the event.
	 * \param a shared pointer to the new robot
	 */
	void add_to_robot_subset(boost::shared_ptr<Robot> new_robot);

	/**
	 * Returns a constant reference to the robot subset.
	 * \return A constant reference to the robot subset.
	 */
	const vector<boost::shared_ptr<Robot> >& get_robot_subset() const;

private:
	/**
	 * The robot subset for this event.
	 */
	vector<boost::shared_ptr<Robot> > robot_subset_;
};

#endif /* LOOK_EVENT_H_ */
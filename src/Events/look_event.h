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
 * look_event.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef LOOK_EVENT_H_
#define LOOK_EVENT_H_

#include <list>
#include <boost/smart_ptr.hpp>
#include "event.h"
#include "../Model/robot.h"


/**
 * \class LookEvent
 * \brief A LookEvent is an event which causes a subset of the robots to recieve new
 *        information about the world
 *
 */
class LookEvent : public Event {
public:

	explicit LookEvent(int time) : Event(time) {}

	/**
	 * Adds a new robot to the subset of robots in the event.
	 * \param a shared pointer to the new robot
	 */
	void add_to_robot_subset(boost::shared_ptr<Robot> new_robot);

	/**
	 * Returns a constant reference to the robot subset.
	 * \return A constant reference to the robot subset.
	 */
	const std::list<boost::shared_ptr<Robot> >& robot_subset() const;

private:
	/**
	 * The robot subset for this event.
	 */
	std::list<boost::shared_ptr<Robot> > robot_subset_;
};

#endif /* LOOK_EVENT_H_ */

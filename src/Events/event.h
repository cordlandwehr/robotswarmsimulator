/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 * event.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef EVENT_H_
#define EVENT_H_

class Event {
public:
	explicit Event(int time) : time_(time) {}
	virtual ~Event() = 0;

	int time() const { return time_; }
private:
	/**
	 * The time the event happens
	 */
	int time_;
};

#endif /* EVENT_H_ */

/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

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
 * History.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef HISTORY_H_
#define HISTORY_H_

#include <cstddef>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread/mutex.hpp>

#include "time_point.h"

class TimePoint;

/**
 * \brief History of the simulation
 * The history class maintains a circular buffer to store past simulation states.
 * It provides thread safe accessor to the buffer.
 */
class History {
public:
	/**
	 * creates a new history with given size
	 */
	History(std::size_t size);

	/**
	 * inserts a new TimePoint object into the history
	 */
	void insert(boost::shared_ptr<TimePoint> time_point);

	/**
	 * gets the oldest TimePoint object in the buffer as copy and marks it as consumed.
	 * If no consumeable TimePoint is available either the thread is blocked (blocked = true)
	 * or a NULL pointer is returned (block = false).
	 */
	boost::shared_ptr<TimePoint> get_oldest_unused(bool block = false);

	/**
	 * returns a const reference to the newest TimePoint object in the buffer.
	 * Should never be called if there is another thread which might call push_back()
	 * concurrently
	 */
	const TimePoint& get_newest() const;

	// .. other accessors may follow
	const std::size_t size() {return history_.size();}
	const std::size_t capacity() {return history_.capacity();}
private:
	boost::circular_buffer<boost::shared_ptr<TimePoint> > history_;
	std::size_t consumer_position_;
	boost::interprocess::interprocess_semaphore empty_count_;
	boost::interprocess::interprocess_semaphore fill_count_;
	boost::mutex mutex_;
};

#endif /* HISTORY_H_ */

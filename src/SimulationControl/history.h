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

class WorldInformation;

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
	 * inserts a new WorldInformation object into the history
	 */
	void insert(boost::shared_ptr<WorldInformation> world_information);

	/**
	 * gets the oldest WorldInformation object in the buffer as copy and marks it as consumed.
	 * If no consumeable WorldInformation is available either the thread is blocked (blocked = true)
	 * or a NULL pointer is returned (block = false).
	 */
	boost::shared_ptr<WorldInformation> get_oldest_unused(bool block = false);

	/**
	 * returns a const reference to the newest world information object in the buffer.
	 * Should never be called if there is another thread which might call push_back()
	 * concurrently
	 */
	const WorldInformation& get_newest() const;

	// .. other accessors may follow
	const std::size_t size() {return history_.size();}
	const std::size_t capacity() {return history_.capacity();}
private:
	boost::circular_buffer<boost::shared_ptr<WorldInformation> > history_;
	std::size_t consumer_position_;
	boost::interprocess::interprocess_semaphore empty_count_;
	boost::interprocess::interprocess_semaphore fill_count_;
	boost::mutex mutex_;
};

#endif /* HISTORY_H_ */

/*
 * History.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef HISTORY_H_
#define HISTORY_H_

#include <boost/smart_ptr.hpp>
#include <boost/circular_buffer.hpp>

class WorldInformation;

/**
 * \brief History of the simulation
 * The history class maintains a circular buffer to store past simulation states.
 * It provides thread safe accessor to the buffer.
 */
class History {
public:
	/**
	 * inserts a new WorldInformation object into the history
	 */
	void push_back(boost::shared_ptr<WorldInformation> world_information);

	/**
	 * gets the oldest WorldInformation object in the buffer and marks it as consumed
	 */
	boost::shared_ptr<WorldInformation> get_oldest();

	// .. other accessors may follow
private:
	boost::circular_buffer<boost::shared_ptr<WorldInformation> > history_;
};

#endif /* HISTORY_H_ */

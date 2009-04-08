/*
 * atomic_semisynchronous_asg.h
 *
 *  Created on: Apr 8, 2009
 *      Author: craupach
 */

#ifndef ATOMIC_SEMISYNCHRONOUS_ASG_H_
#define ATOMIC_SEMISYNCHRONOUS_ASG_H_

#include <vector>
#include <boost/smart_ptr.hpp>

#include "activation_sequence_generator.h"

class Robot;

class AtomicSemisynchronousASG : public ActivationSequenceGenerator {
public:
	AtomicSemisynchronousASG() : time_of_next_event(0), current_state(AtomicSemisynchronousASG::look) {}
	/**
	 * Initializes the ASG.
	 * \param The history
	 */
	void initialize(const History& history, const vector<boost::shared_ptr<Robot> >& robots);

	/**
	 * Returns the next event.
	 * \return The next event produced by the ASG
	 */
	boost::shared_ptr<Event> get_next_event();

	/**
	 * Returns the time the next event happens
	 * \return Integer representing the next time an event will happen
	 */
	int get_time_of_next_event() {return time_of_next_event;};

private:
	enum State {
		look,
		compute,
		move
	};

	int time_of_next_event;
	State current_state;
	boost::shared_ptr<Robot> cur_robot;
};


#endif /* ATOMIC_SEMISYNCHRONOUS_ASG_H_ */

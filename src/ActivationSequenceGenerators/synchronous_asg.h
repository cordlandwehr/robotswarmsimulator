/*
 * SynchronousASG.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef SYNCHRONOUSASG_H_
#define SYNCHRONOUSASG_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/circular_buffer.hpp>

#include "activation_sequence_generator.h"
#include "../Model/world_information.h"
#include "../Events/look_event.h"
#include "../Requests/request.h"

using namespace std;

/**
 * \class SynchronousASG
 * \brief The synchronous ASG produces a sequence of events according to the fully-synchronous time-model.
 */
class SynchronousASG : public ActivationSequenceGenerator {
public:
	SynchronousASG(): time_of_next_event_(0) {};

	/**
	 * initializes the synchronous ASG from the given intial world_state. Needs to be called before the
	 * ASG is used
	 * \param The intial world state
	 */
	void initialize(boost::shared_ptr<WorldInformation> world_state);

	/**
	 * Returns the next event. Since the ASG is synchronous the sequence of events will have the form
	 * of (look-compute-handle_requests)*. Each look and each compute event applies to every existing robot.
	 * In each handle_requests event all outstanding requests from the previous compute event is given to
	 * the event handler.
	 * \return The next event in the sequence.
	 */
	boost::shared_ptr<Event> get_next_event();

	/**
	 * Returns the time of the next event. This is always the time of the last event plus one.
	 * \return The time of the next event.
	 */
	int get_time_of_next_event() {return time_of_next_event_;};

	/**
	 * Updates the sequence of events. For the synchronous ASG this only stores the requests of robots
	 * stored in compute events. The requests are added to the next handle_requests event.
	 * \param A pointer to the history of world states.
	 * \param The last handled event
	 */
	void update_sequence(boost::shared_ptr<boost::circular_buffer <WorldInformation> > history,
				         boost::shared_ptr<Event> last_event);

private:
	/**
	 * The time the next event will happen.
	 */
	int time_of_next_event_;

	/**
	 * A set of unhandled requests from the last compute event.
	 */
	vector<boost::shared_ptr<Request> > unhandled_request_set_;
};

#endif /* SYNCHRONOUSASG_H_ */

//
//  synchronous_asg_wm.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 16.07.11.
//

#ifndef SYNCHRONOUS_ASG_WM_H_
#define SYNCHRONOUS_ASG_WM_H_

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/circular_buffer.hpp>

#include "activation_sequence_generator.h"
#include "../Model/world_information.h"
#include "../Events/look_event.h"
#include "../Requests/request.h"
#include "../Model/robot_identifier.h"


// forward declarations
class History;

/**
 * \class SynchronousASGWM
 * \brief The synchronous ASG produces a sequence of events according to the fully-synchronous time-model.
 */
class SynchronousASGWM : public ActivationSequenceGenerator {
public:
	SynchronousASGWM(boost::shared_ptr<WorldInformation> initial_world_information): time_of_next_event_(0), world_information(initial_world_information)  {}
    
	/**
	 * initializes the synchronous ASG from the given intial world_state. Needs to be called before the
	 * ASG is used
	 * \param The intial world state
	 */
	void initialize(
		    const History& history,
		    const std::vector<boost::shared_ptr<Robot> >& robots,
		    const std::vector<boost::shared_ptr<WorldModifier> >& world_modifiers = *new std::vector<boost::shared_ptr<WorldModifier> >());
	
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
	int get_time_of_next_event() { return time_of_next_event_; }
    
	/**
	 * Updates the sequence of events. For the synchronous ASG this only stores the requests of robots
	 * stored in compute events. The requests are added to the next handle_requests event.
	 * \param A constant refrence to the newest world information
	 * \param The last handled event
	 */
	void update(TimePoint& time_point,
			    boost::shared_ptr<Event> event);
    
private:
	/**
	 * The time the next event will happen.
	 */
	int time_of_next_event_;
    
	/**
	 * A set of unhandled requests from the last compute event.
	 */
	std::vector<boost::shared_ptr<const Request> > unhandled_request_set_;
    
	/*/**
	 * The set of all robots
	 */
	//std::vector<boost::shared_ptr<Robot> > robots_;

	/**
	 * The set of all world modifiers
	 */
	std::vector<boost::shared_ptr<WorldModifier> > world_modifiers_;
	
	/*
	 * The current world information.
	*/
	boost::shared_ptr<WorldInformation> world_information;
    
	// constants for readability
    static const int kTimeToModify = 0;
	static const int kTimeToLook = 1;
	static const int kTimeToCompute = 2;
	static const int kTimeToMove = 3;
	static const int kNumberOfEvents = 4;
};

#endif // SYNCHRONOUS_ASG_WM_H_

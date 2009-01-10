/*
 * activation_sequence_generator.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef ACTIVATION_SEQUENCE_GENERATOR_H_
#define ACTIVATION_SEQUENCE_GENERATOR_H_

#include <boost/smart_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include "../Model/world_information.h"
#include "../Events/look_event.h"
#include "../SimulationKernel/simulation_listener.h"

using namespace std;

// forward declarations
class History;

/**
 * \class ActivationSequenceGenerator
 * \brief Interface for activation sequence generators
 *
 * The activation sequence generator (AGS) decides, according to some user--specified rules,
 * how to time the different events. In other words, it manages the timing of the execution
 * of the robot algorithms.
 *
 * The ActivationSequenceGenerator class inherits from the SimulationListener interface.
 *
 */
class ActivationSequenceGenerator : public SimulationListener {


public:

	virtual ~ActivationSequenceGenerator() {};

	/**
	 * Initializes the ASG.
	 * \param The history
	 */
	virtual void initialize(const History& history) = 0;

	/**
	 * Returns the next event.
	 * \return The next event produced by the ASG
	 */
	virtual boost::shared_ptr<Event> get_next_event() = 0;

	/**
	 * Returns the time the next event happens
	 * \return Integer representing the next time an event will happen
	 */
	virtual int get_time_of_next_event() = 0;

};

#endif /* ACTIVATION_SEQUENCE_GENERATOR_H_ */

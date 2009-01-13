/*
 * Event.cc
 *
 *  Created on: Jan 13, 2009
 *      Author: craupach
 */

#include "event.h"

/*
 * Implementation of pure virtual destructor.
 * Needs to be done because derived classes will call it if they are destructed.
 * Is pure virtual to mark Event as an abstract base class.
 */
Event::~Event() {}

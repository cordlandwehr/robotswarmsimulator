//
//  world_modifier_event.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 16.07.11.
//

#ifndef WORLD_MODIFIER_EVENT_H_
#define WORLD_MODIFIER_EVENT_H_

#include <list>
#include <boost/smart_ptr.hpp>
#include "event.h"
#include "../Model/world_modifier.h"


/**
 * \class WorldModifierEvent
 * \brief A WorldModifierEvent is an event which causes a set of WorldModifier objects
 *        to apply changes to the world.
 */
class WorldModifierEvent : public Event {
public:
    
	explicit WorldModifierEvent(int time) : Event(time) {}
    
	/**
	 * Adds a new WorldModifier to the set of modifiers in the event.
	 * \param a shared pointer to the new WorldModifier
	 */
	void add_to_world_modifier_set(boost::shared_ptr<WorldModifier> new_world_modifier);
    
	/**
	 * Returns a constant reference to the WorldModifier set.
	 * \return A constant reference to the WorldModifier set.
	 */
	const std::list<boost::shared_ptr<WorldModifier> >& world_modifier_set() const;
    
private:
	/**
	 * The WorldModifier set for this event.
	 */
	std::list<boost::shared_ptr<WorldModifier> > world_modifier_set_;
};

#endif /* WORLD_MODIFIER_EVENT_H_ */
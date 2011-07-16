//
//  world_modifier_event.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 16.07.11.
//

#include "world_modifier_event.h"

void WorldModifierEvent::add_to_world_modifier_set(boost::shared_ptr<WorldModifier> new_world_modifier) {
	world_modifier_set_.push_back(new_world_modifier);
}

const std::list<boost::shared_ptr<WorldModifier> >& WorldModifierEvent::world_modifier_set() const {
	return world_modifier_set_;
}

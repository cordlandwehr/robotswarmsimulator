//
//  default_world_modifier.cc
//  Robot Swarm Simulator
//
//  Created by xynodes on 28.08.11.
//

#include "default_world_modifier.h"


DefaultWorldModifier::DefaultWorldModifier(){

}

std::string DefaultWorldModifier::get_algorithm_id() const{
	return "DEFAULT_WORLD_MODIFIER";
}

std::set< boost::shared_ptr<Request> >
DefaultWorldModifier::compute(const boost::shared_ptr<WorldInformation> &world_information) {
	//do nothing

	std::set< boost::shared_ptr<Request> > result;
	return result;
}

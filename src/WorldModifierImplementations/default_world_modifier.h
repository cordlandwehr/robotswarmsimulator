//
//  default_world_modifier.h
//  Robot Swarm Simulator
//
//  Created by xynodes on 28.08.11.
//

#ifndef DEFAULT_WORLD_MODIFIER_H_
#define DEFAULT_WORLD_MODIFIER_H_

#include <string>
#include <set>

#include <boost/smart_ptr.hpp>

#include "layouter.h"
#include "../Wrapper/boost_graph_wrapper.h"

class WorldInformation;

class DefaultWorldModifier : public Layouter {
public:
	DefaultWorldModifier();

	virtual std::string get_algorithm_id() const;

	virtual std::set< boost::shared_ptr<Request> >
    compute(const boost::shared_ptr<WorldInformation> &world_information);

};

#endif /* DEFAULT_WORLD_MODIFIER_H_ */

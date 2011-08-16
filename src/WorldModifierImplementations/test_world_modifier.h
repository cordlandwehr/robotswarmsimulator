//
//  test_world_modifier.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 19.07.11.
//

#ifndef TEST_WORLD_MODIFIER_H_
#define TEST_WORLD_MODIFIER_H_

#include <boost/smart_ptr.hpp>
#include <string>
#include <set>

#include "../Model/world_information.h"
#include "../Model/world_modifier.h"

class TestWorldModifier : public WorldModifier {
public:
    virtual std::string get_algorithm_id() const;
    virtual std::set< boost::shared_ptr<Request> >
        compute(const boost::shared_ptr<WorldInformation> &world_information);
};

#endif // TEST_WORLD_MODIFIER_H_

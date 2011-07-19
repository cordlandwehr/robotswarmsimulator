//
//  test_world_modifier.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 19.07.11.
//

#include "../Utilities/console_output.h"

#include "test_world_modifier.h"

std::string TestWorldModifier::get_algorithm_id() const{
    return "TEST_WORLD_MODIFIER";
}

std::set< boost::shared_ptr<Request> > TestWorldModifier::compute() {
    std::set< boost::shared_ptr<Request> > requests;
    // debug message (this should appear only once)
    ConsoleOutput::log(ConsoleOutput::EventHandler, ConsoleOutput::debug)
        << "Hi there!";
    return requests;
}
//
//  world_modifier.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 16.07.11.
//

#include "../Utilities/console_output.h"

#include "../Views/view_factory.h"
#include "../ViewModels/global_view.h"

#include "world_modifier.h"

// tell compiler to initialize static member view_factory_
boost::shared_ptr<AbstractViewFactory> WorldModifier::view_factory_;

boost::shared_ptr<AbstractViewFactory> WorldModifier::get_view_factory() {
    if (!view_factory_) {
        // debug message (this should appear only once)
        ConsoleOutput::log(ConsoleOutput::EventHandler, ConsoleOutput::debug)
            << "Creating new GlobalView ViewFactory for WorldModifiers.";
        // create new factory
        view_factory_.reset(new ViewFactory<GlobalView>);
    }
    return view_factory_;
}

void WorldModifier::update_view(const boost::shared_ptr<WorldInformation> &world_information) {
    view_ = get_view_factory()->create_new_view_instance(world_information);
}

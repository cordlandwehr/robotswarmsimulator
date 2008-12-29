#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../ActivationSequenceGenerators/synchronous_asg.h"
#include "../../Model/world_information.h"
#include "../../Events/look_event.h"
#include "../../Events/event.h"
#include "../../Model/robot.h"


BOOST_AUTO_TEST_CASE(synchronous_asg_smoke_test)
{
	//TODO(craupach) there should be an init here... robots being added and stuff
	boost::shared_ptr<WorldInformation> world_state;

	SynchronousASG synchronous_asg;
	synchronous_asg.initialize(world_state);

	// first event should be at time 0
	BOOST_CHECK(synchronous_asg.get_time_of_next_event() == 0);

	// first event should be a look event
	boost::shared_ptr<Event> next_event = synchronous_asg.get_next_event();
	LookEvent * look_event = dynamic_cast<LookEvent*>(next_event.get());
	BOOST_CHECK(look_event != NULL);

	// second event should be a compute event (for all robots: to be added later)

}

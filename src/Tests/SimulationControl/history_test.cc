#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Events/look_event.h"
#include "../../Events/event.h"
#include "../../SimulationControl/history.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector3d.h"
#include "../Fixtures/simple_world_fixture.h"

BOOST_FIXTURE_TEST_CASE(history_test, SimpleWorldFixture)
{
	boost::shared_ptr<WorldInformation> second_world_information;
	second_world_information.reset(new WorldInformation());
	second_world_information->set_time(1);
	history->push_back(second_world_information);

	// newest shoud be the second world information
	BOOST_CHECK_EQUAL(history->get_newest().time(), 1);

	// oldest should be the initial world information now be the initial one
	BOOST_CHECK_EQUAL(history->get_oldest().time(), 0);

	// newest shoud be the second world information
	BOOST_CHECK_EQUAL(history->get_newest().time(), 1);

	// initial world info should be consumed now. So oldest should be the second world information.
	BOOST_CHECK_EQUAL(history->get_oldest().time(), 1);


}

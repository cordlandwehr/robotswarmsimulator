#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Events/look_event.h"
#include "../../Events/event.h"
#include "../../SimulationControl/history.h"
#include "../../SimulationControl/time_point.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../Fixtures/simple_world_fixture.h"

BOOST_FIXTURE_TEST_CASE(history_test, SimpleWorldFixture)
{
	boost::shared_ptr<WorldInformation> second_world_information;
	second_world_information.reset(new WorldInformation());
	second_world_information->set_time(1);
	boost::shared_ptr<TimePoint> second_time_point(new TimePoint());
	second_time_point->set_world_information(second_world_information);

	history->insert(second_time_point);

	// newest shoud be the second world information
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 1);

	// oldest should be the initial world information now be the initial one
	BOOST_CHECK_EQUAL(history->get_oldest_unused()->world_information_ptr()->time(), 0);

	// newest shoud be the second world information
	BOOST_CHECK_EQUAL(history->get_newest().world_information().time(), 1);

	// initial world info should be consumed now. So oldest should be the second world information.
	BOOST_CHECK_EQUAL(history->get_oldest_unused()->world_information_ptr()->time(), 1);


}

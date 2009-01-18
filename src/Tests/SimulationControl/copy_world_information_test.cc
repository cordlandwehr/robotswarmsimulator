#include <iostream>
#include <iomanip>
#include <ctime>
#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>


#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Utilities/vector3d.h"
#include "../Fixtures/simple_world_fixture.h"

const unsigned kCopyCount = 1233;

BOOST_FIXTURE_TEST_CASE(copy_world_information_test, SimpleWorldFixture)
{
	//Run this test in release mode

	//create 1000 Robots and RobotDatas
	for(unsigned i = 3; i < 1000; i++) {
		boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
		boost::shared_ptr<Robot> robot(new SimpleRobot(id));
		boost::shared_ptr<Vector3d> pos(new Vector3d());
		pos->insert_element(kXCoord,0.0);
		pos->insert_element(kYCoord,0.0);
		pos->insert_element(kZCoord,0.0);
		boost::shared_ptr<RobotData> robotData(new RobotData(id, pos, *robot));
		initial_world_information->add_robot_data(robotData);
	}

	BOOST_CHECK_EQUAL(initial_world_information->robot_data().size(), 1000);

	//copy them
	//Note: WorldInformations added to array so optimizer hopefully does not remove the copying.
	boost::array<boost::shared_ptr<WorldInformation>, kCopyCount> array;

	long start_time = time(NULL);

	for(unsigned i = 0; i < kCopyCount; i++) {
		array[i] = boost::shared_ptr<WorldInformation>(new WorldInformation(*initial_world_information));
	}

	long time_needed = time(NULL)-start_time;

	std::cout << "Total time: " << time_needed << "s." << std::endl;
	std::cout << "Time per copy: " << std::setprecision(5) << time_needed*1000.0/kCopyCount << "ms." << std::endl;
}

#include <iostream>
#include <iomanip>
#include <ctime>
#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/foreach.hpp>


#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../Fixtures/simple_world_fixture.h"
#include "../../Views/view.h"
#include "../../Views/spheric_view.h"
#include "../../Views/parametrized_view_factory.h"

const unsigned kCopyCount = 127;

BOOST_FIXTURE_TEST_CASE(view_performance_test, SimpleWorldFixture)
{
	//Run this test in release mode
	std::srand(47);
	float radius = 2.0;
	//create 1000 Robots and RobotDatas
	for(unsigned i = 3; i < 1000; i++) {
		boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
		boost::shared_ptr<Robot> robot(new SimpleRobot(id));
		boost::shared_ptr<Vector3d> pos(new Vector3d());
		pos->insert_element(kXCoord,std::rand()*1000.0/RAND_MAX);
		pos->insert_element(kYCoord,std::rand()*1000.0/RAND_MAX);
		pos->insert_element(kZCoord,std::rand()*1000.0/RAND_MAX);
		boost::shared_ptr<RobotData> robotData(new RobotData(id, pos, *robot));
		initial_world_information->add_robot_data(robotData);
		robots.push_back(robot);
	}

	BOOST_CHECK_EQUAL(initial_world_information->robot_data().size(), 1000);

	ParametrizedViewFactory<SphericView, double> factory(radius);
	unsigned counter = 0;
	std::cout << "View performance test." << std::endl;
	std::clock_t start_time = clock();

	for(unsigned i = 0; i < kCopyCount; i++) {
		boost::shared_ptr<View> view = factory.create_new_view_instance(initial_world_information);

		BOOST_FOREACH(boost::shared_ptr<Robot> robot, robots) {
			std::set<boost::shared_ptr<RobotIdentifier> > set(view->get_visible_robots(*robot));
			counter += set.size();
		}
		std::cout << "Robots found " << counter << std::endl;
	}

	std::clock_t time_needed = clock()-start_time;
	std::cout << "Total time: " << time_needed/CLOCKS_PER_SEC << "s." << std::endl;
	std::cout << "Time per complete visibility check: " << std::setprecision(5) << time_needed*1000.0/kCopyCount/CLOCKS_PER_SEC << "ms." << std::endl;

	std::vector< boost::shared_ptr<RobotData> > rd = initial_world_information->robot_data();
	int count = 0;
	BOOST_FOREACH( boost::shared_ptr<RobotData> robot, rd){
		BOOST_FOREACH( boost::shared_ptr<RobotData> robot2, rd){

			if(robot2->id() == robot->id()){
				continue;
			}
			const Vector3d pos1 = robot2->position();
			const Vector3d pos = robot->position();
			float x = pos1(0) - pos(0);
			float y = pos1(1) - pos(1);
			float z = pos1(2) - pos(2);
			if( std::sqrt( x*x + y*y + z*z) < radius){
				count++;
			}
		}
	}
	std::cout << "Robots found: " << count << std::endl;

}


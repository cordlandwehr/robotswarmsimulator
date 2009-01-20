/*
 * octree_test.cc
 *
 *  Created on: 15.01.2009
 *      Author: kamil
 */

#include <ctime>
#include <cstdlib>
#include <cstdio>

#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Model/obstacle.h"
#include "../../Model/box.h"
#include "../../Model/sphere.h"
#include "../../Events/look_event.h"
#include "../../Events/event.h"
#include "../../SimulationControl/history.h"
#include "../../EventHandlers/event_handler.h"
#include "../../Utilities/vector3d.h"
#include "../Fixtures/simple_world_fixture.h"
#include "../../Views/octree.h"

BOOST_FIXTURE_TEST_CASE(octree_test, SimpleWorldFixture)
{

/*	std::vector<boost::shared_ptr<RobotData> > robots;
	std::vector<boost::shared_ptr<WorldObject> > markers;
	std::vector<boost::shared_ptr<Obstacle> > obstacles;

	float radius = 2.0;

	// set the starting seed, so we get always the same numbers in rand
	std::srand(47);


	for(int i = 0; i < 1000; i++){

		boost::shared_ptr<RobotData> newRobot= boost::shared_ptr(new RobotData);
		Vector3d robot_pos = Vector3d();

		robot_pos(0) = std::rand()*100/RAND_MAX;
		robot_pos(1) = std::rand()*100/RAND_MAX;
		robot_pos(2) = std::rand()*100/RAND_MAX;

		newRobot->set_position(robot_pos);

		robots.push_back(newRobot);


	}

	float start = std::clock();

	boost::shared_ptr<Octree> octree(new Octree(10,radius*2.0));
	octree->scene_dimensions(markers, obstacles, robots);

	octree->create_tree(markers,obstacles,robots);


	float end = std::clock();

	float ticks = (end - start) / CLOCKS_PER_SEC;
	std::printf("Built time for Octree: %f \n", ticks);

	start = std::clock();

	std::vector<boost::shared_ptr<RobotData> >::iterator it_robots;
	std::vector<boost::shared_ptr<RobotData> >::iterator it_tobots_comp;

	for(it_robots = robots.begin(); it_robots != robots.end(); ++it_robots){
		Vector3d robot_pos = *it_robots->position();

		for(it_robots_comp = robots.begin(); it_robots_comp != robots.end(); ++it_robots_comp){

			Vector3d robot_comp = *it_robots_comp->position();
			float x = robot_pos(0) - robot_comp(0);
			float y = robot_pos(1) - robot_comp(1);
			float z = robot_pos(2) - robot_comp(2);
			float dist = std::sqrt(x*x + y*y + z*z);
			if(dist < radius){
				// put it into an awesome list ;)
			}

		}
	}

	end = std::clock();
	float ticks = (end - start) / CLOCKS_PER_SEC;
	std::printf("time for simple view test: %f \n", ticks); */

}

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
#include <set>

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Model/obstacle.h"
#include "../../Model/box.h"
#include "../../Model/sphere.h"
#include "../../Model/robot_identifier.h"

#include "../../Utilities/vector_arithmetics.h"

#include "../../Views/octree.h"

class Request;

class MyRobot : public Robot{

public:
	MyRobot(boost::shared_ptr<RobotIdentifier> id): Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute(){

		std::set<boost::shared_ptr<Request> > ret;
		return ret;
	}


};


BOOST_AUTO_TEST_CASE(octree_test) {
	std::vector<boost::shared_ptr<RobotData> > robots;
		std::vector<boost::shared_ptr<WorldObject> > markers;
		std::vector<boost::shared_ptr<Obstacle> > obstacles;

		float radius = 2.0;

		// set the starting seed, so we get always the same numbers in rand
		std::srand(47);

		boost::shared_ptr<RobotData> newRobot((RobotData*)0);
		Robot * robot;
		boost::shared_ptr<RobotIdentifier> ident;
		boost::shared_ptr<Vector3d> robot_pos;
		for(int i = 0; i < 1000; i++){


			robot_pos.reset( new Vector3d());
			boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
			//TODO: undefined behavior here, since robot is deleted after each forloop run.
			robot = new MyRobot(id);


			boost::shared_ptr<Vector3d> robot_pos = boost::shared_ptr<Vector3d>(new Vector3d());

			robot_pos->insert_element(kXCoord,std::rand()*1000.0/RAND_MAX);
			robot_pos->insert_element(kYCoord,std::rand()*1000.0/RAND_MAX);
			robot_pos->insert_element(kZCoord,std::rand()*1000.0/RAND_MAX);


			boost::shared_ptr<RobotData> newRobot = boost::shared_ptr<RobotData>(new RobotData(id, robot_pos, *robot));


			robots.push_back(newRobot);


		}

		std::printf("test\n");
		float start = std::clock();
		for(int i = 0; i < 1000; i++) {
		boost::shared_ptr<Octree> octree(new Octree(10,radius*2.0));
		octree->scene_dimensions(markers, obstacles, robots);


		octree->create_tree(markers,obstacles,robots);
		}

		float end = std::clock();

		float ticks = (end - start) / CLOCKS_PER_SEC;
		std::printf("Built time for Octree: %f \n", ticks);

		start = std::clock();

		std::vector<boost::shared_ptr<RobotData> >::iterator it_robots;
		std::vector<boost::shared_ptr<RobotData> >::iterator it_robots_comp;
		for(int i = 0; i < 1000; i++)
		for(it_robots = robots.begin(); it_robots != robots.end(); ++it_robots){
			Vector3d robot_pos = (*it_robots)->position();

			for(it_robots_comp = robots.begin(); it_robots_comp != robots.end(); ++it_robots_comp){

				Vector3d robot_comp = (*it_robots_comp)->position();
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

		ticks = (end - start) / CLOCKS_PER_SEC;
		std::printf("time for simple view test: %f \n", ticks);



}

/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include "../../Views/octree_utilities.h"

class Request;

class MyRobot : public Robot {

public:
	MyRobot(boost::shared_ptr<RobotIdentifier> id, boost::shared_ptr<std::string> algorithm_id)
	: Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute(){

		std::set<boost::shared_ptr<Request> > ret;
		return ret;
	}

	virtual std::string get_algorithm_id () const {
		return "MyRobot";
	}


};

/*
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
			boost::shared_ptr<std::string> tmp_algo(new std::string("NONE"));
			robot = new MyRobot(id, tmp_algo);


			boost::shared_ptr<Vector3d> robot_pos = boost::shared_ptr<Vector3d>(new Vector3d());

			robot_pos->insert_element(kXCoord,std::rand()*1000.0/RAND_MAX);
			robot_pos->insert_element(kYCoord,std::rand()*1000.0/RAND_MAX);
			robot_pos->insert_element(kZCoord,std::rand()*1000.0/RAND_MAX);


			boost::shared_ptr<RobotData> newRobot = boost::shared_ptr<RobotData>(new RobotData(id, robot_pos, *robot));


			robots.push_back(newRobot);


		}

		std::printf("test\n");
		float start = std::clock();

		boost::shared_ptr<Octree> octree(new Octree(10,radius*2.0));


		octree->create_tree(markers,obstacles,robots);


		float end = std::clock();

		float ticks = (end - start) / CLOCKS_PER_SEC;
		std::printf("Built time for Octree: %f \n", ticks);

		start = std::clock();
		std::vector<boost::shared_ptr<RobotData> >::iterator it_robs;
		for(it_robs = robots.begin(); it_robs != robots.end(); ++it_robs ){

			OctreeUtilities::get_visible_markers_by_radius(octree, (*it_robs)->position(), radius );
		}


		end = std::clock();
		ticks = (end - start )/ CLOCKS_PER_SEC;
		std::printf("Search time: %f \n", ticks);



		start = std::clock();

		std::vector<boost::shared_ptr<RobotData> >::iterator it_robots;
		std::vector<boost::shared_ptr<RobotData> >::iterator it_robots_comp;



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
*/

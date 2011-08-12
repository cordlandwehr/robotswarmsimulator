/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
 * circle_formation_generator.cc
 *
 *  Created on: Apr 30, 2009
 *      Author: craupach
 */
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "circle_formation_generator.h"

void CircleFormationGenerator::init(const boost::program_options::variables_map& vm) {
	radius_ = vm["distr-pos-circle"].as<double>();
	starting_angle_ = vm["distr-pos-circle-angle"].as<double>();
}

void CircleFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	distribute_robots_circle(robotDataList);
}

void CircleFormationGenerator::distribute_robots_circle(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;
	int circle_num = 0;
	double cur_angle = 0.0;
	double incr_angle = starting_angle_;
	const long double pi = 3.1415926535897932384626433832795028841968; // awkward.

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		if(circle_num == 0) {
			// center robot
			boost::shared_ptr<Vector3d> center(new Vector3d());
			center->insert_element(kXCoord,0.0);
			center->insert_element(kYCoord,0.0);
			center->insert_element(kZCoord,0.0);
			(*iter)->set_position( center );

			// this robot is a chieftain
			(*iter)->set_type( MASTER );
			circle_num++;
		} else {
			// position robot on circle i at cur_angle
			boost::shared_ptr<Vector3d> pos(new Vector3d());
			pos->insert_element(kXCoord,std::cos(cur_angle));
			pos->insert_element(kYCoord,std::sin(cur_angle));
			pos->insert_element(kZCoord,0.0);
			(*pos) *= (circle_num * radius_);
			(*iter)->set_position( pos );

			// adjust counter variables
			cur_angle += incr_angle;
			if(cur_angle > 2 * pi) {
				circle_num++;
				cur_angle = 0.0;
				incr_angle /= 2;
			}
		}
	}
}

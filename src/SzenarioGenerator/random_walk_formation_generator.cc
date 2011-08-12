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
 * random_walk_formation_generator.cc
 *
 *  Created on: May 13, 2009
 *      Author: craupach
 */

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "random_walk_formation_generator.h"

void RandomWalkFormationGenerator::init(const boost::program_options::variables_map& vm) {

	steps_ = vm["rnd-walk-steps"].as<unsigned int>();
	min_distance_ = vm["rnd-min-distance"].as<double>();
	max_distance_ = vm["rnd-max-distance"].as<double>();

	png_.reset(new DistributionGenerator(vm["seed"].as<unsigned int>()));

	// needed to get the direction of the next random step
	png_->init_uniform_on_sphere(3);

	// needed for the step distance
	png_->init_uniform_real(min_distance_, max_distance_);

}

void RandomWalkFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {

	std::vector< boost::shared_ptr<RobotData> >::iterator iter;
	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		// for each robot do a random walk
		Vector3d total;
		total(kXCoord) = 0.0;
		total(kYCoord) = 0.0;
		total(kZCoord) = 0.0;
		for(int i = 0; i < steps_; i++) {
			Vector3d next = png_->get_value_uniform_on_sphere_3d();
			next *= png_->get_value_uniform_real();
			total += next;
		}
		boost::shared_ptr<Vector3d> total_ptr(new Vector3d(total));
		(*iter)->set_position(total_ptr);
	}
}

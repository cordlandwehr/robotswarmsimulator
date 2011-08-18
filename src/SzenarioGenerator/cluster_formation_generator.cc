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
 * cluster_formation_generator.cc
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

#include "cluster_formation_generator.h"

void ClusterFormationGenerator::init(const boost::program_options::variables_map& vm) {

	clusters_ = vm["clt-clusters"].as<unsigned int>();
	min_distance_ = vm["clt-min-distance"].as<double>();
	max_distance_ = vm["clt-max-distance"].as<double>();

	png_.reset(new DistributionGenerator(vm["seed"].as<unsigned int>()));

	// needed to get the direction of the next random step
	png_->init_uniform_on_sphere(3);

	// needed for the step distance
	png_->init_uniform_real(min_distance_, max_distance_);

	// needed for distributing the robots
	sigma_ = vm["clt-sigma"].as<double>();
	png_->init_normal(0.0, sigma_);
}

void ClusterFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	// 1. Determine the Cluster Centres
	std::vector<Vector3d> centres;
	for(int i = 0; i < clusters_; i++) {
		Vector3d cur_center = png_->get_value_uniform_on_sphere_3d() * png_->get_value_uniform_real();
		centres.push_back(cur_center);
	}

	// 2. Distribute the robots evenly around the centres
	int target_center = 0;
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;
	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {

		Vector3d total;
		// gaussian distribution with mean 0
		total(kXCoord) = png_->get_value_normal();
		total(kYCoord) = png_->get_value_normal();
		total(kZCoord) = png_->get_value_normal();

		// add robot to its cluster
		total += centres.at(target_center % centres.size());

		boost::shared_ptr<Vector3d> total_ptr(new Vector3d(total));
		(*iter)->set_position(total_ptr);

		target_center++;
	}
}

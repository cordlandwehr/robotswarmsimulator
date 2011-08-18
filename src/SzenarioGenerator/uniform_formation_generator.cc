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
 * uniform_formation_generator.cc
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

#include "uniform_formation_generator.h"

void UniformFormationGenerator::init(const boost::program_options::variables_map& vm) {
	// Source of randomness
	png_.reset(new DistributionGenerator(vm["seed"].as<unsigned int>()));

	// Bounding box
	boundingBox_.insert_element(kXCoord,vm["distr-pos"].as<double>());
	boundingBox_.insert_element(kYCoord,vm["distr-pos"].as<double>());
	boundingBox_.insert_element(kZCoord,vm["distr-pos"].as<double>());

	// Initial velocities
	if (vm["min-vel"].as<double>() != 0.0 || vm["max-vel"].as<double>() != 0.0) {
		distrVel_ = true;
		minVel_ = vm["min-vel"].as<double>();
		maxVel_ = vm["max-vel"].as<double>();
	}

	// Initial accelerations
	if (vm["min-acc"].as<double>() != 0.0 || vm["max-acc"].as<double>() != 0.0) {
		distrAcc_ = true;
		minAcc_ = vm["min-acc"].as<double>();
		maxAcc_ = vm["max-acc"].as<double>();
	}
}

void UniformFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	distribute_robots_uniform(robotDataList);
	if(distrVel_) {
		distribute_velocity_uniform(robotDataList);
	}

	if(distrAcc_) {
		distribute_acceleration_uniform(robotDataList);
	}
}

void UniformFormationGenerator::distribute_robots_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_uniform_real(-boundingBox_(kXCoord)/2, boundingBox_(kXCoord)/2);
		newRandomPosition->insert_element(kXCoord,png_->get_value_uniform_real());
		png_->init_uniform_real(-boundingBox_(kYCoord)/2, boundingBox_(kYCoord)/2);
		newRandomPosition->insert_element(kYCoord,png_->get_value_uniform_real());
		png_->init_uniform_real(-boundingBox_(kZCoord)/2, boundingBox_(kZCoord)/2);
		newRandomPosition->insert_element(kZCoord,png_->get_value_uniform_real());

		(*iter)->set_position( newRandomPosition );
	}
}

void UniformFormationGenerator::distribute_velocity_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		// get vector uniform on unit sphere
		png_->init_uniform_on_sphere(3);
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d(png_->get_value_uniform_on_sphere_3d()));

		// get vector length uniform in range [minVelocity,maxVelocity]
		png_->init_uniform_real(minVel_, maxVel_);
		double vecLength = png_->get_value_uniform_real();
		newRandomPosition->insert_element(kXCoord,vecLength * (*newRandomPosition)(kXCoord));
		newRandomPosition->insert_element(kYCoord,vecLength * (*newRandomPosition)(kYCoord));
		newRandomPosition->insert_element(kZCoord,vecLength * (*newRandomPosition)(kZCoord));

		(*iter)->set_velocity( newRandomPosition );
	}
}

void UniformFormationGenerator::distribute_acceleration_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		// get vector uniform on unit sphere
		png_->init_uniform_on_sphere(3);
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d(png_->get_value_uniform_on_sphere_3d()));

		// get vector length uniform in range [minAcc,maxAcc]
		png_->init_uniform_real(minAcc_, maxAcc_);
		double vecLength = png_->get_value_uniform_real();
		newRandomPosition->insert_element(kXCoord,vecLength * (*newRandomPosition)(kXCoord));
		newRandomPosition->insert_element(kYCoord,vecLength * (*newRandomPosition)(kYCoord));
		newRandomPosition->insert_element(kZCoord,vecLength * (*newRandomPosition)(kZCoord));

		(*iter)->set_acceleration( newRandomPosition );
	}
}

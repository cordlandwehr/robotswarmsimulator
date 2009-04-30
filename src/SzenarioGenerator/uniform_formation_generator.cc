/*
 * uniform_formation_generator.cc
 *
 *  Created on: Apr 30, 2009
 *      Author: craupach
 */

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"

#include "uniform_formation_generator.h"

void UniformFormationGenerator::init(const boost::program_options::variables_map& vm) {
	// Source of randomness
	png_.reset(new DistributionGenerator(vm["seed"].as<unsigned int>());

	// Bounding box
	boundingBox.insert_element(kXCoord,vm["distr-pos"].as<double>());
	boundingBox.insert_element(kYCoord,vm["distr-pos"].as<double>());
	boundingBox.insert_element(kZCoord,vm["distr-pos"].as<double>());

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

void UniformFormationGenerator:distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	distribute_robots_uniform(robotDataList);
	if(distrVec_) {
		distribute_velocity_uniform(robotDataList);
	}

	if(distrAcc_) {
		distribute_acceleration_uniform(robotDataList);
	}
}

void UniformFormationGenerator::distribute_robots_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_uniform_real(-boundingBox(kXCoord)/2, boundingBox(kXCoord)/2);
		newRandomPosition->insert_element(kXCoord,png_->get_value_uniform_real());
		png_->init_uniform_real(-boundingBox(kYCoord)/2, boundingBox(kYCoord)/2);
		newRandomPosition->insert_element(kYCoord,png_->get_value_uniform_real());
		png_->init_uniform_real(-boundingBox(kZCoord)/2, boundingBox(kZCoord)/2);
		newRandomPosition->insert_element(kZCoord,png_->get_value_uniform_real());

		(*iter)->set_position( newRandomPosition );
	}
}

void UniformFormationGenerator::distribute_velocity_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList_.end() ; iter++ ) {
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

	for(iter = robotDataList.begin(); iter != robotDataList_.end() ; iter++ ) {
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

/*
 * gaussian_formation_generator.cc
 *
 *  Created on: May 2, 2009
 *      Author: craupach
 */


#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"
#include "../Utilities/console_output.h"

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "gaussian_formation_generator.h"

void GaussianFormationGenerator::init(const boost::program_options::variables_map& vm) {
	// Source of randomness
	png_.reset(new DistributionGenerator(vm["seed"].as<unsigned int>()));

	// Bounding box
	boundingBox_.insert_element(kXCoord,vm["distr-gauss-pos"].as<double>());
	boundingBox_.insert_element(kYCoord,vm["distr-gauss-pos"].as<double>());
	boundingBox_.insert_element(kZCoord,vm["distr-gauss-pos"].as<double>());

	// Mean and Sigma
	if(vm.count("mean-pos") && vm.count("sigma-pos")) {
		pos_mean_ = vm["mean-pos"].as<double>();
		pos_sigma_ = vm["sigma-pos"].as<double>();
	} else {
		ConsoleOutput::log(ConsoleOutput::Parser, ConsoleOutput::warning) << "no mean and / or sigma given";
		pos_mean_ = vm["distr-gauss-pos"].as<double>() / 2.0;
		pos_sigma_ = 42.0; // as per RFC 2549
	}

	// Initial velocities
	if (vm.count("sigma-vel") && vm.count("mean-vel") &&
		vm["sigma-vel"].as<double>() > 0.0 && vm["mean-vel"].as<double>() != 0.0) {
		distrVel_ = true;
		vel_sigma_ = vm["sigma-vel"].as<double>();
		vel_mean_ = vm["mean-vel"].as<double>();
	}

	// Initial accelerations
	if (vm.count("sigma-acc") && vm.count("mean-acc") &&
		vm["sigma-acc"].as<double>() != 0.0 && vm["mean-acc"].as<double>() != 0.0) {
		distrAcc_ = true;
		acc_mean_ = vm["mean-acc"].as<double>();
		acc_sigma_ = vm["sigma-acc"].as<double>();
	}
}

void GaussianFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	distribute_robots_gaussian(robotDataList);
	if(distrVel_) {
		distribute_velocity_gaussian(robotDataList);
	}

	if(distrAcc_) {
		distribute_acceleration_gaussian(robotDataList);
	}
}


void GaussianFormationGenerator::distribute_robots_gaussian(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_normal(pos_mean_, pos_sigma_);		// set up the generator
		double randomValue;					// random value to be generated

		/*
		 * generates values according normal distribution until value inside bounding box is found
		 * if one side ob box is equal to zero no limit is asssumed, cause probability to be zero is exactly 0 ;)
		 */
		do {
			randomValue = png_->get_value_normal();
			newRandomPosition->insert_element(kXCoord,randomValue);
		} while (
				randomValue + boundingBox_(kXCoord)/2.0 > boundingBox_(kXCoord) ||
				randomValue + boundingBox_(kXCoord)/2.0 < 0.0 ||
			 	boundingBox_(kXCoord)==0.0
			 	);
		do {
			randomValue = png_->get_value_normal();
			newRandomPosition->insert_element(kYCoord,randomValue);
		} while (
				randomValue + boundingBox_(kYCoord)/2.0 > boundingBox_(kYCoord) ||
				randomValue + boundingBox_(kYCoord)/2.0 < 0.0 ||
			 	boundingBox_(kYCoord)==0.0
			 	);
		do {
			randomValue = png_->get_value_normal();
			newRandomPosition->insert_element(kZCoord,randomValue);
		} while (
				randomValue + boundingBox_(kZCoord)/2.0 > boundingBox_(kZCoord) ||
				randomValue + boundingBox_(kZCoord)/2.0 < 0.0 ||
			 	boundingBox_(kZCoord)==0.0
			 	);

		(*iter)->set_position( newRandomPosition );
	}
}



void GaussianFormationGenerator::distribute_velocity_gaussian(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_normal(vel_mean_, vel_sigma_);		// set up the generator

		newRandomPosition->insert_element(kXCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kYCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kZCoord,png_->get_value_normal());

		(*iter)->set_velocity( newRandomPosition );
	}
}

void GaussianFormationGenerator::distribute_acceleration_gaussian(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_normal(acc_mean_, acc_sigma_);		// set up the generator

		newRandomPosition->insert_element(kXCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kYCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kZCoord,png_->get_value_normal());

		(*iter)->set_acceleration( newRandomPosition );
	}
}

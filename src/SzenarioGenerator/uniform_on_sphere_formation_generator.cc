/*
 * uniform_on_sphere_formation_generator.cc
 *
 *  Created on: Aug 24, 2009
 *      Author: phoenixx
 */
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "uniform_on_sphere_formation_generator.h"

void UniformOnSphereFormationGenerator::init(const boost::program_options::variables_map& vm) {
	png_.reset(new DistributionGenerator(vm["seed"].as<unsigned int>()));
	radius_ = vm["distr-uniform-on-sphere"].as<double>();
}

void UniformOnSphereFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	distribute_robots_uniform_on_sphere(robotDataList);
}

void UniformOnSphereFormationGenerator::distribute_robots_uniform_on_sphere(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	png_->init_uniform_on_sphere(3);

	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {

		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());

		Vector3d randomOnSphere = png_->get_value_uniform_on_sphere_3d();

		newRandomPosition->insert_element(kXCoord,randomOnSphere(kXCoord)*radius_);
		newRandomPosition->insert_element(kYCoord,randomOnSphere(kYCoord)*radius_);
		newRandomPosition->insert_element(kZCoord,randomOnSphere(kZCoord)*radius_);

		(*iter)->set_position( newRandomPosition );
	}
}

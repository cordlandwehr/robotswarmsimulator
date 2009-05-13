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

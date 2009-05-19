/*
 * poisson_disc_formation_generator.cc
 *
 *  Created on: May 19, 2009
 *      Author: craupach
 */

#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"
#include "../Utilities/console_output.h"

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "poisson_disc_formation_generator.h"
#include "../Utilities/PoissonDiscSampler/PDSampling.h"

void PoissonDiscFormationGenerator::init(const boost::program_options::variables_map& vm) {
	double radius = 0.0009; // this controls how many robots will be generated
	double robot_diameter = 0.15; // the real diameter of a robot
	scaling_factor_ = robot_diameter / radius;

	if (radius<0.0005 || radius>.2) {
		// do some error handling
	}

	/*
	 * use the boundary sampling method since according to
	 *
	 * A Spatial Data Structure for Fast Poisson-Disk Sample Generation
	 * (ACM Transactions on Graphics, 2006)
	 *
	 * it is the fastest and bestest.
	 */
	sampler_.reset(new BoundarySampler(radius, true, 12));
}

void PoissonDiscFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	sampler_->complete();
	sampler_->maximize();
	int num_points_generated = sampler_->points.size();

	if(num_points_generated < robotDataList.size()) {
		// catastrophe
	}

	std::vector< boost::shared_ptr<RobotData> >::iterator iter;
	int i = 0;
	for(iter = robotDataList.begin(); iter != robotDataList.end() ; iter++ ) {
		// for each robot do a random walk
		Vector3d total;
		total(kXCoord) = sampler_->points.at(i).x;
		total(kYCoord) = sampler_->points.at(i).y;
		total(kZCoord) = 0.0;
		total *= scaling_factor_;

		boost::shared_ptr<Vector3d> total_ptr(new Vector3d(total));
		(*iter)->set_position(total_ptr);
		i += num_points_generated / robotDataList.size();
	}

}

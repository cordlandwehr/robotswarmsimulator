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
	if(!vm.count("seed") || !vm.count("pd-spread") || !vm.count("pd-diameter")) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error)  << "";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "alone again";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "with the wind in the pines";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "somewhere else";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error)  << "(missing parameters for pd generator)";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error)  << "";
	}

	double radius = vm["pd-spread"].as<double>(); // this controls how many robots will be generated
	double robot_diameter = vm["pd-diameter"].as<double>(); // the real diameter of a robot
	scaling_factor_ = robot_diameter / radius;

	if (radius < 0.0005 || radius > 0.2) {
		// do some error handling
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::warning)  << "";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::warning) << "sunny sea winds";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::warning) << "shoulders ache";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::warning) << "for the want of wings";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::warning) << "(Poisson Disc Parameter outside of recommended interval)";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::warning)  << "";
	}

	/*
	 * use the boundary sampling method since according to
	 *
	 * A Spatial Data Structure for Fast Poisson-Disk Sample Generation
	 * (ACM Transactions on Graphics, 2006)
	 *
	 * it is the fastest and bestest.
	 */
	sampler_.reset(new BoundarySampler(radius, true, vm["seed"].as<unsigned int>()));
}

void PoissonDiscFormationGenerator::distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) {
	sampler_->complete();
	sampler_->maximize();
	std::size_t num_points_generated = sampler_->points.size();

	if(num_points_generated < robotDataList.size()) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error)  << "";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "Greedy man,";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "steal steal steal,";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "mad with power";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error)  << "(too many robots were requested for this spread)";
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error)  << "";
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

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
 * gaussian_formation_generator.h
 *
 *  Created on: May 2, 2009
 *      Author: craupach
 */

#ifndef GAUSSIAN_FORMATION_GENERATOR_H_
#define GAUSSIAN_FORMATION_GENERATOR_H_

#include "../Utilities/vector_arithmetics.h"
#include "formation_generator.h"

/**
 * A formation generator which distributes the robot positions gaussian.
 * May also be used to distribute Acceleration and Velocity gaussian.
 */
class GaussianFormationGenerator : public FormationGenerator {
public:
	GaussianFormationGenerator() : FormationGenerator(), distrVel_(false), distrAcc_(false) {}

	/**
	 * initializes the formation generator with parameters from the given map
	 */
	void init(const boost::program_options::variables_map& vm);

	/**
	 * distributes the robot. Init needs to be called before this is called.
	 */
	void distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

private:
	void distribute_robots_gaussian(std::vector< boost::shared_ptr<RobotData> >& robotDataList);
	void distribute_velocity_gaussian(std::vector< boost::shared_ptr<RobotData> >& robotDataList);
	void distribute_acceleration_gaussian(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

	/**
	 * The box where the robots are distributed in
	 */
	Vector3d boundingBox_;
	double pos_mean_;
	double pos_sigma_;

	/**
	 * Parameters for distributing the velocity
	 */
	bool distrVel_;
	double vel_mean_;
	double vel_sigma_;

	/**
	 * Parameters for distributing the acceleration
	 */
	bool distrAcc_;
	double acc_mean_;
	double acc_sigma_;

	/**
	 * A source of randomness
	 */
	boost::shared_ptr< DistributionGenerator > png_;
};

#endif /* GAUSSIAN_FORMATION_GENERATOR_H_ */

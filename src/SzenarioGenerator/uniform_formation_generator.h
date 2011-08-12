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
 * uniform_formation_generator.h
 *
 *  Created on: Apr 30, 2009
 *      Author: craupach
 */

#ifndef UNIFORM_FORMATION_GENERATOR_H_
#define UNIFORM_FORMATION_GENERATOR_H_

#include "../Utilities/vector_arithmetics.h"
#include "formation_generator.h"

/**
 * A formation generator which distributes the robot positions uniform.
 * May also be used to distribute Acceleration and Velocity uniform.
 */
class UniformFormationGenerator : public FormationGenerator {
public:
	UniformFormationGenerator() : FormationGenerator(), distrVel_(false), distrAcc_(false) {}

	/**
	 * initializes the formation generator with parameters from the given map
	 */
	void init(const boost::program_options::variables_map& vm);

	/**
	 * distributes the robot. Init needs to be called before this is called.
	 */
	void distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

private:
	void distribute_robots_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

	/**
	 * TODO(craupach) These should become part of the formation generator abstract class itself
	 */
	void distribute_velocity_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList);
	void distribute_acceleration_uniform(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

	/**
	 * The box where the robots are distributed in
	 */
	Vector3d boundingBox_;

	/**
	 * Parameters for distributing the velocity
	 */
	bool distrVel_;
	double minVel_;
	double maxVel_;

	/**
	 * Parameters for distributing the acceleration
	 */
	bool distrAcc_;
	double minAcc_;
	double maxAcc_;

	/**
	 * A source of randomness
	 */
	boost::shared_ptr< DistributionGenerator > png_;
};

#endif /* UNIFORM_FORMATION_GENERATOR_H_ */

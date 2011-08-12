/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
 * uniform_on_sphere_formation_generator.h
 *
 *  Created on: Aug 26, 2009
 *      Author: phoenixx
 */

#ifndef UNIFORM_ON_SPHERE_FORMATION_GENERATOR_H_
#define UNIFORM_ON_SPHERE_FORMATION_GENERATOR_H_

#include "formation_generator.h"

/**
 * Formation Generator that generates robot positions uniformly distributed on sphere
 */
class UniformOnSphereFormationGenerator : public FormationGenerator {
public:
	/**
	 * initializes the formation generator with parameters from the given map
	 */
	virtual void init(const boost::program_options::variables_map& vm);

	/**
	 * distributes the robot. Init needs to be called before this is called.
	 */
	virtual void distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

private:
	void distribute_robots_uniform_on_sphere(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

	/**
	 * A source of randomness
	 */
	boost::shared_ptr< DistributionGenerator > png_;
	double radius_;
};

#endif /* UNIFORM_ON_SPHERE_FORMATION_GENERATOR_H_ */

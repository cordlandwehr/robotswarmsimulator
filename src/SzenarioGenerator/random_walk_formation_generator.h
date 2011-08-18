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
 * random_walk_formation_generator.h
 *
 *  Created on: May 13, 2009
 *      Author: craupach
 */

#ifndef RANDOM_WALK_FORMATION_GENERATOR_H_
#define RANDOM_WALK_FORMATION_GENERATOR_H_

#include "formation_generator.h"

class RobotData;

/**
 * Formation Generator which generates an initial formation by letting each robot do a random walk.
 */
class RandomWalkFormationGenerator : public FormationGenerator {
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

	/**
	 * number of random steps
	 */
	int steps_;

	/**
	 * minimal distance of a random step
	 */
	double min_distance_;

	/**
	 * maximal distance of a random step
	 */
	double max_distance_;

	/**
	 * A source of randomness
	 */
	boost::shared_ptr< DistributionGenerator > png_;
};

#endif /* RANDOM_WALK_FORMATION_GENERATOR_H_ */

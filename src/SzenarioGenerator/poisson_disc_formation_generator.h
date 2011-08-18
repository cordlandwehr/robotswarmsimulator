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
 * poisson_disc_formation_generator.h
 *
 *  Created on: May 19, 2009
 *      Author: craupach
 */

#ifndef POISSON_DISC_FORMATION_GENERATOR_H_
#define POISSON_DISC_FORMATION_GENERATOR_H_


#include "../Utilities/vector_arithmetics.h"
#include "formation_generator.h"

class PDSampler;

class PoissonDiscFormationGenerator : public FormationGenerator {
public:
	PoissonDiscFormationGenerator() : FormationGenerator() {}

	/**
	 * initializes the formation generator with parameters from the given map
	 */
	void init(const boost::program_options::variables_map& vm);

	/**
	 * distributes the robot. Init needs to be called before this is called.
	 */
	void distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

private:
	boost::shared_ptr<PDSampler> sampler_;
	double scaling_factor_;
};


#endif /* POISSON_DISC_FORMATION_GENERATOR_H_ */

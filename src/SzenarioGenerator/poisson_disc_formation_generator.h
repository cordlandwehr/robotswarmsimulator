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

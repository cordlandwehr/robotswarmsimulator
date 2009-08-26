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

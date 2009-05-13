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

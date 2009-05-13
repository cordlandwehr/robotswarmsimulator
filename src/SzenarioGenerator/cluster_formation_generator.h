/*
 * cluster_formation_generator.h
 *
 *  Created on: May 13, 2009
 *      Author: craupach
 */

#ifndef CLUSTER_FORMATION_GENERATOR_H_
#define CLUSTER_FORMATION_GENERATOR_H_

#include "formation_generator.h"

class RobotData;

/**
 * Formation Generator which generates an initial formation by a gaussian distribution around a given number
 * of cluster centres.
 */
class ClusterFormationGenerator : public FormationGenerator {
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
	 * number of clusters
	 */
	int clusters_;

	/**
	 * minimal distance of clusters from origin
	 */
	double min_distance_;

	/**
	 * maximal distance of clusters from origin
	 */
	double max_distance_;

	// sigma of gaussian distribution around cluster centres
	double sigma_;

	/**
	 * A source of randomness
	 */
	boost::shared_ptr< DistributionGenerator > png_;
};


#endif /* CLUSTER_FORMATION_GENERATOR_H_ */

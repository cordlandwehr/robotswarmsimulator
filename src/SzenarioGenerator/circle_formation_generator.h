/*
 * circle_formation_generator.h
 *
 *  Created on: Apr 30, 2009
 *      Author: craupach
 */

#ifndef CIRCLE_FORMATION_GENERATOR_H_
#define CIRCLE_FORMATION_GENERATOR_H_

#include "formation_generator.h"

class CircleFormationGenerator : public FormationGenerator {
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
	void distribute_robots_circle(std::vector< boost::shared_ptr<RobotData> >& robotDataList);

	/**
	 * radius of the innermost circle
	 */
	double radius_;

	/**
	 * angle between two robots on the innermost circle
	 */
	double starting_angle_;
};

#endif /* CIRCLE_FORMATION_GENERATOR_H_ */

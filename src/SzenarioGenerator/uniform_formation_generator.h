/*
 * uniform_formation_generator.h
 *
 *  Created on: Apr 30, 2009
 *      Author: craupach
 */

#ifndef UNIFORM_FORMATION_GENERATOR_H_
#define UNIFORM_FORMATION_GENERATOR_H_

#include "../Utilities/vector_arithmetics.h"

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

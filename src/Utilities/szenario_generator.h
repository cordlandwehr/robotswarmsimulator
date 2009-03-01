/*
 * szenario_generator.h
 *
 *  Created on: Jan 19, 2009
 *      Author: phoenixx
 */

#ifndef SZENARIO_GENERATOR_H_
#define SZENARIO_GENERATOR_H_

#include <boost/unordered_map.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "parser.h"
#include "vector_arithmetics.h"
#include "distribution_generator.h"

class szenario_generator {
public:
	/**
	 * Initializes the szenario generator with number_robots
	 * @param number_robots int
	 * @param algorithm-name for robot algorithm
	 */
	void init(int number_robots, std::string algorithm_id);

	/**
	 * Method distributes robots interior of a bounding box uniformly
	 * @param boundingBox Vector3d the spans the bounding box
	 */
	void distribute_robots_uniform(Vector3d boundingBox);

	/**
	 * Method distributes robots interior of a bounding box by normal distribution
	 * Generates values according normal distribution until value inside bounding box is found.
	 * If one side ob box is equal to zero no limit is asssumed, cause probability to be zero is exactly 0 ;)
	 * @param boundingBox Vector3d the spans the bounding box
	 * @param mean double value for normal distribution
	 * @param sigma double value for normal distribution
	 */
	void distribute_robots_normal(Vector3d boundingBox, double mean, double sigma);

	/**
	 * Method distributes robot initial velocities interior of sphere with max-radius, but outside sphere with min-radius
	 * @param minVelocity double sets minimum velocity
	 * @param maxVelocity double sets maximum velocity
	 */
	void distribute_velocity_uniform(double minVelocity, double maxVelocity);

	/**
	 * Method distributes robot initial velocities by normal distribution
	 * @param mean double value for normal distribution
	 * @param sigma double value for normal distribution
	 */
	void distribute_velocity_normal(double mean, double sigma);

	/**
	 * Method distributes robot initial accelerations interior of sphere with max-radius, but outside sphere with min-radius
	 * @param minAcc double sets minimum acceleration
	 * @param maxAcc double sets maximum acceleration
	 */
	void distribute_acceleration_uniform(double minAcc, double maxAcc);

	/**
	 * Method distributes robot initial accelerations by normal distribution
	 * @param mean double value for normal distribution
	 * @param sigma double value for normal distribution
	 */
	void distribute_acceleration_normal(double mean, double sigma);


	/**
	 * Method generates random coordinate system, each axis is distributed uniformly at random on 3-dim sphere
	 */
	void distribute_coordsys_uniform();

	/**
	 * Set filename for output, without file extension
	 */
	void set_robotFile(string filename);

	/**
	 * Set filename for output, without file extension
	 */
	void set_obstacleFile(string filename);

	/**
	 * Set filename for output, without file extension
	 */
	void set_worldFile(string filename);

	/**
	 * Set project name
	 */
	void set_projectName(string projectName);

	/**
	 * Set ASG
	 */
	void set_asg(string asg);

	/**
	 * Set compass model
	 */
	void set_compassModel(string compassModel);

	/**
	 * Set statistics template
	 */
	void set_statisticsTemplate(string statisticsTemplate);

	/**
	 * Set statistics subsets
	 */
	void set_statisticsSubsets(string statisticsSubsets);

	/**
	 * Writes everything to files
	 */
	void write_to_file();

	szenario_generator(int seed);
	virtual ~szenario_generator();

	/**
	 * Creates unordered list of coordinates for coordinates for roboters
	 * \param number number of robots
	 * \param boundingBox dimensions of box, the robots should be included in
	 */
	void generate_coords_uniform(Vector3d boundingBox);

private:
	boost::shared_ptr< DistributionGenerator > png_;
	boost::shared_ptr< Parser > parser_;
	boost::shared_ptr< WorldInformation > generatedWorld_;

	std::vector< boost::shared_ptr<Robot> > robotList_;
	std::vector< boost::shared_ptr<RobotData> > robotDataList_;

	boost::shared_ptr< std::string > projectName_;
	boost::shared_ptr< std::string > projectFileName_;
	boost::shared_ptr< std::string > robotFileName_;
	boost::shared_ptr< std::string > obstacleFileName_;
	boost::shared_ptr< std::string > worldFileName_;
	boost::shared_ptr< std::string > compassModel_;
	boost::shared_ptr< std::string > statisticsTemplate_;
	boost::shared_ptr< std::string > statisticsSubsets_;
	boost::shared_ptr< std::string > asg_;
	boost::shared_ptr< std::string > default_algorithm_;

};

#endif /* SZENARIO_GENERATOR_H_ */

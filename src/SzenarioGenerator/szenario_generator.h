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
#include <boost/program_options.hpp>

#include <vector>
#include <string>

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "../Utilities/parser.h"
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"

class FormationGenerator;

class ScenarioGenerator {
public:
	/**
	 * Initializes the szenario generator with number_robots
	 * @param number_robots int
	 * @param algorithm-name for robot algorithm
	 */
	void init(const boost::program_options::variables_map& vm);

	/**
	 * Creates a new initial formation according to the set formation generator
	 */
	void distribute();



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
	 * Method distributes robot initial velocities by normal distribution
	 * @param mean double value for normal distribution
	 * @param sigma double value for normal distribution
	 */
	void distribute_velocity_normal(double mean, double sigma);

	/**
	 * Method distributes robot initial accelerations by normal distribution
	 * @param mean double value for normal distribution
	 * @param sigma double value for normal distribution
	 */
	void distribute_acceleration_normal(double mean, double sigma);


	/**
	 * Method generates random coordinate systems with:
	 *  - scaled: if x, y, z requested
	 *  - rotated: if x, y, z requested
	 *  @param vm parameter-map with parameter from command-line, theses are: cosys-scale-{x,y,z}, cosys-rotate-{x,y,z}
	 */
	void distribute_coordsys(const boost::program_options::variables_map& vm);

	/**
	 * Set filename for output, without file extension
	 */
	void set_robotFile(std::string filename);

	/**
	 * Set filename for output, without file extension
	 */
	void set_obstacleFile(std::string filename);

	/**
	 * Set filename for output, without file extension
	 */
	void set_worldFile(std::string filename);

	/**
	 * Set project name
	 */
	void set_projectName(std::string projectName);

	/**
	 * Set statistics template
	 */
	void set_statisticsTemplate(std::string statisticsTemplate);

	/**
	 * Set statistics subsets
	 */
	void set_statisticsSubsets(std::string statisticsSubsets);

	/**
	 * Sets position request handler for project file.
	 * This is only a pre-configured standard handler that should be further configured in created swarm-file.
	 */
	void add_play_pos_request_handler();

	/**
	 * Sets velocity request handler for project file.
	 * This is only a pre-configured standard handler that should be further configured in created swarm-file.
	 */
	void add_play_vel_request_handler();

	/**
	 * Sets acceleration request handler for project file.
	 * This is only a pre-configured standard handler that should be further configured in created swarm-file.
	 */
	void add_play_acc_request_handler();

	/**
	 * Writes everything to files
	 */
	void write_to_file();

	ScenarioGenerator(int seed);
	virtual ~ScenarioGenerator();

	/**
	 * Creates unordered list of coordinates for coordinates for roboters
	 * \param number number of robots
	 * \param boundingBox dimensions of box, the robots should be included in
	 */
	void generate_coords_uniform(Vector3d boundingBox);

private:
	void init_formation_generator(const boost::program_options::variables_map& vm);

	boost::shared_ptr< DistributionGenerator > png_;
	boost::shared_ptr< Parser > parser_;

	/**
	 * A generator to distribute position, velocity and acceleration
	 */
	boost::shared_ptr< FormationGenerator> formation_generator_;


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

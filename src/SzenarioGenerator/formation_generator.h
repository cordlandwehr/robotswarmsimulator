/*
 * formation_generator.h
 *
 *  Created on: Apr 30, 2009
 *      Author: craupach
 */

#ifndef FORMATION_GENERATOR_H_
#define FORMATION_GENERATOR_H_

#include <boost/program_options.hpp>

class RobotData;

/**
 * Abstract class for formation generation
 */
class FormationGenerator {
public:
	virtual ~FormationGenerator() {};
	
	/**
	 * initializes the formation generator with parameters from the given map
	 */
	virtual void init(const boost::program_options::variables_map& vm) = 0;

	/**
	 * distributes the robot. Init needs to be called before this is called.
	 */
	virtual void distribute(std::vector< boost::shared_ptr<RobotData> >& robotDataList) = 0;

};

#endif /* FORMATION_GENERATOR_H_ */

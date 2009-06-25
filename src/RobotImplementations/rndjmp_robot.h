/*
 * rndjmp_robot.h
 *
 *  Created on: Feb 27, 2009
 *      Author: sven
 */

#ifndef RNDJMP_ROBOT_H_
#define RNDJMP_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <cmath>

#include "../Utilities/distribution_generator.h"

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

#include <iostream>

class RndJmpRobot : public Robot {
public:
	static unsigned int rndInit;
	RndJmpRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id) {
		rand = boost::shared_ptr<DistributionGenerator>(new DistributionGenerator(rndInit));
		rndInit += 123123;
		rand->init_normal(0, M_PI/4.0);
	}

	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "RndJmpRobot";
	}

private:
	boost::shared_ptr<DistributionGenerator> rand;
	double get_min_dist(Vector3d & tp, std::vector<Vector3d> & positions);
};


#endif /* RNDJMP_ROBOT_H_ */

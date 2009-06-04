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

#include "../Utilities/distribution_generator.h"

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

#include <iostream>

class RndJmpRobot : public Robot {
public:

	RndJmpRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id) {
		rand = boost::shared_ptr<DistributionGenerator>(new DistributionGenerator(123));
		rand->init_normal(0, 1);
	}

	std::set<boost::shared_ptr<Request> > compute();
	void cut_trivial(Vector3d & tp, std::vector<Vector3d> & positions, double v);
	void cut_maxmove(Vector3d & tp, std::vector<Vector3d> & positions, double v);

	virtual std::string get_algorithm_id () const {
		return "RndJmpRobot";
	}

private:
	boost::shared_ptr<DistributionGenerator> rand;
};


#endif /* RNDJMP_ROBOT_H_ */

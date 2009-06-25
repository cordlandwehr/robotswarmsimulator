/*
 * rndjmp_robot.h
 *
 *  Created on: Feb 27, 2009
 *      Author: sven
 */

#ifndef POT_ROBOT_H_
#define POT_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

#include "../Utilities/distribution_generator.h"

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

#include <iostream>

class PotRobot : public Robot {
public:

	PotRobot(boost::shared_ptr<RobotIdentifier> id, std::string potfunc) : Robot(id) {
		rand = boost::shared_ptr<DistributionGenerator>(new DistributionGenerator(123));
		rand->init_uniform_on_sphere(3);
		rand->init_uniform(1, 100000);

		potfunc_id_ = 3;
		/* TODO latter
		if (potfunc=="A") {
			potfunc_id_=1;
		} else {
			std::cout << "unknown potfunc: " << potfunc << " set to default" << std::endl;
			potfunc_id_=0;
		}*/

		// TODO atm very bad: the robot's view-radius is hardcoded, because I don't know how to read it.
		v_ = 30;
		finalMinDist_ = 0.2;
		aimDist_ = 0.4;
		maxTries_ = 1000;
		DEBUG = false;
		LOGINFO = true;
	}

	std::set<boost::shared_ptr<Request> > compute();
	void cut_trivial(Vector3d & tp, std::vector<Vector3d> & positions, double v);
	void cut_maxmove(Vector3d & tp, std::vector<Vector3d> & positions, double v);

	double calc_pot_A(Vector3d & tp, std::vector<Vector3d> & others);
	double calc_pot_B(Vector3d & tp, std::vector<Vector3d> & others);
	double calc_pot_C(Vector3d & tp, std::vector<Vector3d> & others);

	virtual std::string get_algorithm_id () const {
		return "PotRobot";
	}

private:
	boost::shared_ptr<DistributionGenerator> rand;
	int potfunc_id_;

	double calc_pot(Vector3d & tp, std::vector<Vector3d> & others);

	bool DEBUG;
	bool LOGINFO;
	double v_;
	double finalMinDist_, aimDist_;
	double maxTries_;
	double curMinDist_, curMaxDist_, curAvgDist_;

};


#endif /* POT_ROBOT_H_ */

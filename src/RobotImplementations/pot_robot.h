/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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
	static unsigned int rndInit;

	PotRobot(boost::shared_ptr<RobotIdentifier> id, std::string potfunc) : Robot(id) {
		rand = boost::shared_ptr<DistributionGenerator>(new DistributionGenerator(rndInit));
		rndInit += 123123;
		rand->init_uniform_on_sphere(3);
		rand->init_uniform(1, 100000);

		finalMinDist_ = 0.25;
		aimDist_ = 0.25;
		maxTries_ = 10;
		DEBUG = false;
		LOGINFO = false;

		if (potfunc=="1") {
			potfunc_id_=1;
		} else if (potfunc=="2") {
			potfunc_id_=2;
		} else if (potfunc=="3") {
			potfunc_id_=3;
			maxTries_ = 100;
		} else if (potfunc=="4") {
			potfunc_id_=4;
			maxTries_ = 1;
		} else {
			std::cout << "unknown potfunc: " << potfunc << " set to default" << std::endl;
			potfunc_id_=1;
		}
	}

	std::set<boost::shared_ptr<Request> > compute();
	void cut_trivial(Vector3d & tp, std::vector<Vector3d> & positions, double v);
	void cut_maxmove(Vector3d & tp, std::vector<Vector3d> & positions, double v);

	double calc_pot_1(Vector3d & tp, std::vector<Vector3d> & others);
	double calc_pot_2(Vector3d & tp, std::vector<Vector3d> & others);
	double calc_pot_3(Vector3d & tp, std::vector<Vector3d> & others);
	double calc_pot_4(Vector3d & tp, std::vector<Vector3d> & others);

	virtual std::string get_algorithm_id () const {
		return "PotRobot";
	}

private:
	boost::shared_ptr<DistributionGenerator> rand;
	int potfunc_id_;

	double calc_pot(Vector3d & tp, std::vector<Vector3d> & others);

	bool DEBUG;
	bool LOGINFO;

	double finalMinDist_, aimDist_;
	double maxTries_;
	double v_;
	double curMinDist_, curMaxDist_, curAvgDist_;

};


#endif /* POT_ROBOT_H_ */

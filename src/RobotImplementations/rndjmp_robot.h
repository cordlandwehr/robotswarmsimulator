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
	RndJmpRobot(boost::shared_ptr<RobotIdentifier> id, std::string paramset) : Robot(id) {
		rand = boost::shared_ptr<DistributionGenerator>(new DistributionGenerator(rndInit));
		rndInit += 123123;

		if (paramset=="0") {
			paramset_ = 0;
		} else if (paramset=="1") {
			paramset_ = 1;
		} else if (paramset=="2") {
			paramset_ = 2;
		} else if (paramset=="3") {
			paramset_ = 3;
		} else if (paramset=="4") {
			paramset_ = 4;
		} else if (paramset=="5") {
			paramset_ = 5;
		} else if (paramset=="6") {
			paramset_ = 6;
		} else {
			std::cout << "unknown paramset: " << paramset << " set to default " << std::endl;
			paramset_ = 1;
		}

		log_ = false;
		switch(paramset_) {
			case 0 : // direct move with NO random
				m_ = 0.25;
				s_ = 3.0/2.0*m_;
				alpha_ = 0;
				freeze_ = 0;
				break;
			case 1 : // move with extreme random PI/2
				m_ = 0.25;
				s_ = 3.0/2.0*m_;
				alpha_ = M_PI/2;
				freeze_ = 0;
				break;
			case 2 : // move with good random PI/4
				m_ = 0.25;
				s_ = 3.0/2.0*m_;
				alpha_ = M_PI/4.0;
				freeze_ = 0;
				break;
			case 3 : // move with good random PI/4 AND sphere = should-distance
				s_ = 0.25;
				m_ = 2.0/3.0*s_;
				alpha_ = M_PI/4.0;
				freeze_ = 0;
				break;
			case 4 : // move with freeze 0.25
				s_ = 0.25;
				m_ = 2.0/3.0*s_;
				alpha_ = M_PI/4.0;
				freeze_ = 0.25;
				break;
			case 5 : // move with freeze 0.5
				s_ = 0.25;
				m_ = 2.0/3.0*s_;
				alpha_ = M_PI/4.0;
				freeze_ = 0.5;
				break;
			case 6 :  // move with freeze 0.5 AND logging
				s_ = 0.25;
				m_ = 2.0/3.0*s_;
				alpha_ = M_PI/4.0;
				freeze_ = 0.5;
				log_ = true;
				break;
		}

		s3_ = s_ * s_ * s_ / 8.0;
		m3_ = m_ * m_ * m_;

		rand->init_normal(0, alpha_);

		eps_ = 0;
	}

	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "RndJmpRobot";
	}

private:
	boost::shared_ptr<DistributionGenerator> rand;
	double get_min_dist(Vector3d & tp, std::vector<Vector3d> & positions);

	int paramset_;
	double m_;
	double s_;
	double alpha_;
	double freeze_;

	double s3_;
	double m3_;
	double eps_;
	bool log_;
};


#endif /* RNDJMP_ROBOT_H_ */

/*
 * cog_robot.h
 *
 *  Created on: Feb 27, 2009
 *      Author: craupach
 */

#ifndef COG_ROBOT_H_
#define COG_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

#include <iostream>

class COGRobot : public Robot {
public:
	COGRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "COGRobot";
	}
};


#endif /* COG_ROBOT_H_ */

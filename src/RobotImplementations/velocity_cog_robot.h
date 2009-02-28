/*
 * velocity_cog_robot.h
 *
 *  Created on: Feb 28, 2009
 *      Author: craupach
 */

#ifndef VELOCITY_COG_ROBOT_H_
#define VELOCITY_COG_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

#include <iostream>

class VelocityCOGRobot : public Robot {
public:
	VelocityCOGRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "VelocityCOGRobot";
	}
};


#endif /* VELOCITY_COG_ROBOT_H_ */

/*
 * miniball_robot.h
 *
 *  Created on: Mar 1, 2009
 *      Author: craupach
 */

#ifndef MINIBALL_ROBOT_H_
#define MINIBALL_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

class MiniballRobot : public Robot {
public:
	MiniballRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "MiniballRobot";
	}
};

#endif /* MINIBALL_ROBOT_H_ */

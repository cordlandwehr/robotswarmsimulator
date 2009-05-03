/*
 * ch_robot.h
 *
 *  Created on: Mar 20, 2009
 *      Author: martinah
 */

#ifndef CH_ROBOT_H_
#define CH_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

class CHRobot : public Robot {
public:
	CHRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id) {}

	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "CHRobot";
	}
};

#endif /* ACCELERATION_COG_ROBOT_H_ */

/*
 * acceleration_cog_robot.h
 *
 *  Created on: Feb 28, 2009
 *      Author: craupach
 */

#ifndef ACCELERATION_COG_ROBOT_H_
#define ACCELERATION_COG_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

#include "../Model/robot.h"

class RobotIdentifier;
class Request;

class AccelerationCOGRobot : public Robot {
public:
	AccelerationCOGRobot(boost::shared_ptr<RobotIdentifier> id) : Robot(id), max_acceleration_(0.5) {}
	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		return "AccelerationCOGRobot";
	}
private:
	double max_acceleration_;
};

#endif /* ACCELERATION_COG_ROBOT_H_ */

/*
 * simple_robot.h
 *
 *  Created on: Feb 10, 2009
 *      Author: craupach
 */

#ifndef SIMPLE_ROBOT_H_
#define SIMPLE_ROBOT_H_

#include "../Model/robot.h"

class SimpleRobot : public Robot {
public:
	SimpleRobot(boost::shared_ptr<RobotIdentifier> id)
			: Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute() {

		// don't do stuff
		std::set<boost::shared_ptr<Request> > result_set;

		return result_set;
	}

	virtual std::string get_algorithm_id () const {
		return "SimpleRobot";
	}
};


#endif /* SIMPLE_ROBOT_H_ */

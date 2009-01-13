/*
 * request.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "../Model/robot.h"

/**
 * \class Request
 * \brief Class which represents a request of a robot.
 *
 */
class Request {
public:
	explicit Request(boost::shared_ptr<Robot> robot) : robot_(robot) {}
	virtual ~Request() = 0;

	/**
	 * returns a constant reference to the robot which issued the request
	 * \Return a constant reference to the robot which issued the request
	 */
	const Robot& robot() const {return *robot_;};

private:
	/**
	 * The robot which issued the request.
	 */
	boost::shared_ptr<Robot> robot_;
};

#endif /* REQUEST_H_ */

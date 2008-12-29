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
private:
	/**
	 * The robot which issued the request.
	 */
	boost::shared_ptr<Robot> robot_;
};

#endif /* REQUEST_H_ */

/**
 * \brief	TODO insert description here
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <boost/shared_ptr.hpp>

class RobotIdentifier;

class Robot {
public:
	Robot();
	virtual ~Robot();

	const boost::shared_ptr<RobotIdentifier>& id() const {
		return id_;
	}

private:
	boost::shared_ptr<RobotIdentifier> id_;
};

#endif /* ROBOT_H_ */

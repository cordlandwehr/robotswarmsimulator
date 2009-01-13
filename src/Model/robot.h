/**
 * \brief	TODO insert description here
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class RobotIdentifier;
class View;
class Request;

class Robot {
public:
	// TODO(craupach) temporary fix. No way to set identifier before.
	Robot(boost::shared_ptr<RobotIdentifier> id) : id_(id) {};
	virtual ~Robot();

	// TODO(craupach) temporary fix. This should really returning a const reference but then view.cc doesn't compile
	// and I don't want to change it.
	boost::shared_ptr<RobotIdentifier> id() const {
		return id_;
	}

	void set_view(const boost::weak_ptr<View>& view);

	virtual std::set<boost::shared_ptr<Request> > compute() = 0;
private:
	boost::shared_ptr<RobotIdentifier> id_;
	boost::weak_ptr<View> view_;
};

#endif /* ROBOT_H_ */

/**
 * \brief	TODO insert description here
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <set>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class RobotIdentifier;
class View;
class Request;

class Robot {
public:

	explicit Robot(boost::shared_ptr<RobotIdentifier> id) : id_(id) {};
	virtual ~Robot();


	const boost::shared_ptr<RobotIdentifier>& id() const {
		return id_;
	}

	void set_view(const boost::shared_ptr<View>& view);

	virtual std::string get_algorithm_id () const = 0;

	virtual std::set<boost::shared_ptr<Request> > compute() = 0;
protected:
	boost::shared_ptr<View> view_;


private:
	boost::shared_ptr<RobotIdentifier> id_;	

};

#endif /* ROBOT_H_ */

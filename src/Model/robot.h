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

	explicit Robot(boost::shared_ptr<RobotIdentifier> id, boost::shared_ptr<std::string> algorithm_id)
		: id_(id), algorithm_id_(algorithm_id) {};
	virtual ~Robot();


	const boost::shared_ptr<RobotIdentifier>& id() const {
		return id_;
	}

	void set_view(const boost::weak_ptr<View>& view);

	const std::string& get_algorithm_id () const;

	virtual std::set<boost::shared_ptr<Request> > compute() = 0;
protected:
	boost::weak_ptr<View> view_;


private:
	boost::shared_ptr<RobotIdentifier> id_;
	boost::shared_ptr<std::string> algorithm_id_;

};

#endif /* ROBOT_H_ */

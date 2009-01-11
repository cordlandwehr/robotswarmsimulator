/*
 * robot_control.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef ROBOT_CONTROL_H_
#define ROBOT_CONTROL_H_

#include <set>
#include <boost/shared_ptr.hpp>
#include "simulation_listener.h"

class ViewFactory;
class Request;
class Robot;
/**
 * \class RobotControl
 * \brief Controls the Robots. Mainly assigns Views and calls Robot::compute().
 *
 *
 */

//TODO: Register me as Listener!

class RobotControl: public SimulationListener {
public:
	RobotControl(boost::shared_ptr<ViewFactory> view_factory);
	~RobotControl();

	virtual void update(const WorldInformation& world_information,
				            boost::shared_ptr<Event> last_event);

	std::set<boost::shared_ptr<Request> > compute_new_request(Robot& robot);
	//TODO: I changed the return type from View to void. Adds the View to the given robot by itself.
	void compute_view(Robot& robot);
private:
	boost::shared_ptr<ViewFactory> view_factory_;
};

#endif /* ROBOT_CONTROL_H_ */

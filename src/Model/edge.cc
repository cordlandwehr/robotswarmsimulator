//
//  edge.cc
//  Robot Swarm Simulator
//
//  Created by Jonas on 27.07.11.
//

#include "edge.h"

Edge::~Edge(){
}

boost::shared_ptr<RobotIdentifier> Edge::getRobot1() const {
	return robot1_;
}

boost::shared_ptr<RobotIdentifier> Edge::getRobot2() const {
	return robot2_;
}

boost::shared_ptr<WorldObject> Edge::clone() const {
	return boost::shared_ptr<WorldObject>(new Edge(*this));
}

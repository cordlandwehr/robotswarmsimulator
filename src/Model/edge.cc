//
//  edge.cc
//  Robot Swarm Simulator
//
//  Created by Jonas on 27.07.11.
//

#include "edge.h"

Edge::~Edge(){
}

boost::shared_ptr<RobotIdentifier> Edge::robot1() const {
	return robot1_;
}

boost::shared_ptr<RobotIdentifier> Edge::robot2() const {
	return robot2_;
}

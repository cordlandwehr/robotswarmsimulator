//
//  directed_edge.cc
//  Robot Swarm Simulator
//
//  Created by Jonas on 27.07.11.
//

#include "directed_edge.h"

DirectedEdge::~DirectedEdge(){
}

boost::shared_ptr<RobotIdentifier> DirectedEdge::source() const {
	return Edge::getRobot1();
}

boost::shared_ptr<RobotIdentifier> DirectedEdge::target() const {
	return Edge::getRobot2();
}

//
//  directed_edge.h
//  Robot Swarm Simulator
//
//  Created by Jonas Knoll on 19.07.11.
//

#ifndef DIRECTED_EDGE_H_
#define DIRECTED_EDGE_H_

#include "edge.h"

class DirectedEdge: public Edge {
public:
	DirectedEdge(boost::shared_ptr<Identifier> id,
	             boost::shared_ptr<Vector3d> position,
				 boost::shared_ptr<RobotIdentifier> v1,
				 boost::shared_ptr<RobotIdentifier> v2) : Edge (id, position, v1, v2) {};

	boost::shared_ptr<RobotIdentifier> source() const {
		return getRobot1();
	}

	boost::shared_ptr<RobotIdentifier> target() const {
		return getRobot2();
	}
};

#endif /* DIRECTED_EDGE_H_ */

//
//  directed_edge.h
//  Robot Swarm Simulator
//
//  Created by Jonas on 19.07.11.
//

#ifndef DIRECTED_EDGE_H_
#define DIRECTED_EDGE_H_

#include "edge.h"

class DirectedEdge : public Edge {
public:
	DirectedEdge(boost::shared_ptr<RobotIdentifier> source,
				 boost::shared_ptr<RobotIdentifier> target) : Edge(source, target) {};

	DirectedEdge(const DirectedEdge& rhs) : Edge(rhs) {};

	virtual ~DirectedEdge();

	boost::shared_ptr<RobotIdentifier> source() const;
	boost::shared_ptr<RobotIdentifier> target() const;

	virtual boost::shared_ptr<WorldObject> clone() const;
};

#endif /* DIRECTED_EDGE_H_ */

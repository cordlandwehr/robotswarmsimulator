//
//  undirected_edge.h
//  Robot Swarm Simulator
//
//  Created by Jonas on 19.07.11.
//

#ifndef UNDIRECTED_EDGE_H_
#define UNDIRECTED_EDGE_H_

#include "edge.h"

class Edge;

class UndirectedEdge : public Edge {
public:
	UndirectedEdge(boost::shared_ptr<RobotIdentifier> v1,
	               boost::shared_ptr<RobotIdentifier> v2) : Edge(v1, v2) {};

	UndirectedEdge(const UndirectedEdge& rhs) : Edge(rhs) {};

	virtual ~UndirectedEdge();

	virtual boost::shared_ptr<WorldObject> clone() const;
};

#endif /* UNDIRECTED_EDGE_H_ */

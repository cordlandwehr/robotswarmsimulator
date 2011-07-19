//
//  undirected_edge.h
//  Robot Swarm Simulator
//
//  Created by Jonas Knoll on 19.07.11.
//

#ifndef UNDIRECTED_EDGE_H_
#define UNDIRECTED_EDGE_H_

#include <edge.h>

class UndirectedEdge: public Edge {
public:
	UndirectedEdge(boost::shared_ptr<RobotData> v1, boost::shared_ptr<RobotData> v2) : Edge (v1, v2){};
};

#endif /* UNDIRECTED_EDGE_H_ */

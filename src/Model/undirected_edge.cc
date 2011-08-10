//
//  undirected_edge.cc
//  Robot Swarm Simulator
//
//  Created by Jonas on 27.07.11.
//

#include "undirected_edge.h"

UndirectedEdge::~UndirectedEdge(){
}

boost::shared_ptr<WorldObject> UndirectedEdge::clone() const {
	return boost::shared_ptr<WorldObject>(new UndirectedEdge(*this));
}

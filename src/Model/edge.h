//
//  edge.h
//  Robot Swarm Simulator
//
//  Created by Jonas on 19.07.11.
//

#ifndef EDGE_H_
#define EDGE_H_

#include "world_object.h"
#include "edge_identifier.h"
#include "../Utilities/vector_arithmetics.h"

class Identifier;
class RobotIdentifier;
class EdgeIdentifier;

class Edge : public WorldObject {
public:
	Edge(boost::shared_ptr<RobotIdentifier> r1,
	     boost::shared_ptr<RobotIdentifier> r2)
	: WorldObject(boost::shared_ptr<Identifier>(new EdgeIdentifier()), boost::shared_ptr<Vector3d>(new Vector3d())), robot1_(r1), robot2_(r2)
	{
		;
	}

	virtual ~Edge();

	boost::shared_ptr<RobotIdentifier> getRobot1() const;

	boost::shared_ptr<RobotIdentifier> getRobot2() const;

private:
	boost::shared_ptr<RobotIdentifier> robot1_;
	boost::shared_ptr<RobotIdentifier> robot2_;
};

#endif /* EDGE_H_ */

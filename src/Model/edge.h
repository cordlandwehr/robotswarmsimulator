//
//  edge.h
//  Robot Swarm Simulator
//
//  Created by Jonas Knoll on 19.07.11.
//

#ifndef EDGE_H_
#define EDGE_H_

#include "world_object.h"
#include "robot_data.h"
#include "identifier.h"

class RobotData;

class Edge : public WorldObject {
public:
	Edge(boost::shared_ptr<Identifier> id,
	     boost::shared_ptr<Vector3d> position,
	     boost::shared_ptr<RobotData> r1,
	     boost::shared_ptr<RobotData> r2) : WorldObject(id, position), robot1_(r1), robot2_(r2)
	{
		;
	}

	boost::shared_ptr<RobotData> getRobot1() const {
		return robot1_;
	}

	boost::shared_ptr<RobotData> getRobot2() const {
		return robot2_;
	}

private:
	boost::shared_ptr<RobotData> robot1_;
	boost::shared_ptr<RobotData> robot2_;
};

#endif /* EDGE_H_ */

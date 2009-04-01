/*
 *  collision_position_request_handler.cc
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 31.03.09.
 *  Copyright 2009 Peter Kling. All rights reserved.
 */

#include <boost/foreach.hpp>

#include <EventHandlers/collision_position_request_handler.h>
#include <Model/robot_data.h>
#include <Model/world_information.h>
#include <Requests/position_request.h>


CollisionPositionRequestHandler::CollisionPositionRequestHandler(CollisionStrategy strategy, double clearance,
                                                                  unsigned int seed, double discard_probability,
                                                                  const History& history)
: VectorRequestHandler(seed, discard_probability, history), strategy_(strategy), clearance_(clearance) { }


void CollisionPositionRequestHandler::handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                                               boost::shared_ptr<const Request> request) {
	// this handler does support only position requests
	typedef boost::shared_ptr<const PositionRequest> ConstPositionRequestPtr;
	ConstPositionRequestPtr position_request = boost::dynamic_pointer_cast<const PositionRequest>(request);
	if(!position_request)
		throw std::invalid_argument("Not a position request.");
	
	// get requesting robot
	const boost::shared_ptr<RobotIdentifier>& robot_id = position_request->robot().id();
	RobotData& robot_data = world_information->get_according_robot_data(robot_id);
	
	// handle request, but save robot's current position
	const Vector3d old_robot_position = robot_data.position();
	VectorRequestHandler::handle_request_reliable(world_information, request); // handling itself is done by super class
	
	// TODO(peter): Collisions are currently detected by iterating over all robots in the world. It would be much more
	//    efficient (at least for many robots) if we find colliding robots using a dedicated data structure as for
	//    example an octree. However, we have to be able to change the octree if we change the robot's position (e.g.
	//    remove the object from it's old position in the octree and readd it using it's new position). As far as I've
	//    seen, our Octree class does not yet support such methods.
	typedef boost::shared_ptr<const RobotData> ConstRobotPtr;
	BOOST_FOREACH(const ConstRobotPtr& other_robot_data, world_information->robot_data()) {
		if (other_robot_data.get() == &robot_data) // skip 'self-collision'
			continue;
		
		double distance = boost::numeric::ublas::norm_2(robot_data.position() - other_robot_data->position());
		if (distance < clearance_) { // collision detected
			if (strategy_ == STOP) {
				// undo the (already applied) request
				boost::shared_ptr<Vector3d> old_position_ptr(new Vector3d(old_robot_position));
				robot_data.set_position(old_position_ptr);
			} else if (strategy_ == TOUCH) {
				// TODO(peter): implement
			}
		}
	}
}

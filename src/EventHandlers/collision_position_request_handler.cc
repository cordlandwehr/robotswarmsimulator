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
	RobotData& robot = world_information->get_according_robot_data(robot_id);
	
	// if robot is already collided, we abort (no good way to resolve such collisions)
	if (find_colliding_robot(robot, world_information->robot_data()))
		return;
	
	// handle request, but save robot's current position
	const Vector3d old_position = robot.position();
	VectorRequestHandler::handle_request_reliable(world_information, request); // handling itself is done by super class
	
	// collision handling
	if (strategy_ == STOP) {
		// STOP strategy: reject request on collision (i.e. undo the already applied request)
		if (find_colliding_robot(robot, world_information->robot_data())) {
			boost::shared_ptr<Vector3d> old_position_ptr(new Vector3d(old_position));
			robot.set_position(old_position_ptr);
		}
	} else if (strategy_ == TOUCH) {
		// TOUCH strategy: on collision, move back until colliding robots are touching
		while (const RobotPtr& other_robot = find_colliding_robot(robot, world_information->robot_data())) {
			move_back_to_touchpoint(robot, *other_robot, old_position);
		}
	}
}


// This method essentially solves the quadratic equation for the value alpha that indicates how far the given robot must
// be moved backwards towards it's old_position such that it has distance clearance_ to the other_robot.
// Note that the resulting alpha should always lie in the closed interval [0,1].
void CollisionPositionRequestHandler::move_back_to_touchpoint(RobotData& robot, const RobotData& other_robot,
                                                              const Vector3d& old_position) {
	using namespace boost::numeric::ublas;
	
	// some values needed for the computation
	const Vector3d new_to_old          = old_position - robot.position();
	const Vector3d new_to_other        = other_robot.position() - robot.position();
	const double vector_product        = inner_prod(new_to_old, new_to_other);
	const double new_to_old_dist       = norm_2(new_to_old);
	const double new_to_other_dist     = norm_2(new_to_other);
	const double new_to_old_dist_sqr   = new_to_old_dist*new_to_old_dist;
	const double new_to_other_dist_sqr = new_to_other_dist*new_to_other_dist;
	
	// The result of the general solution term for quadratic equations (splitted in the two summands; note that we need
	// only the 'quadr_sol_left + quadr_sol_right' solution and not the 'quadr_sol_left - quadr_sol_right' (which is
	// negative).
	const double quadr_sol_left = vector_product/new_to_old_dist_sqr;
	const double quadr_sol_right = std::sqrt(  vector_product*vector_product
	                                         + clearance_*clearance_ * new_to_old_dist_sqr
	                                         - new_to_other_dist_sqr * new_to_old_dist_sqr) / new_to_old_dist_sqr;
	const double quadr_solution = quadr_sol_left + quadr_sol_right;
	
	// Due to rounding errors, we have to be somewhat tollerant. More exactly:
	//     - make sure that the solution is <= 1 (if violated, then due to rounding errors!)
	//     - we allow/force an (very small) epsilon gap between two objects, to make sure that these objects are not
	//       considered as colliding again (would cause a livelock)
	const double epsilon = 1E-12; // force a gap of (maximum) size 10^(-12) between the two objects
	const double alpha = std::min(1., quadr_solution + epsilon/new_to_old_dist);
	
	// place robot at corresponding position
	boost::shared_ptr<Vector3d> corrected_position(new Vector3d(robot.position()+alpha*new_to_old));
	robot.set_position(corrected_position);
}


CollisionPositionRequestHandler::RobotPtr CollisionPositionRequestHandler::find_colliding_robot(const RobotData& robot, const RobotContainer& robots) {
	// TODO(peter): Collisions are currently detected by iterating over all robots in the world. It would be much more
	//    efficient (at least for many robots) if we find colliding robots using a dedicated data structure as for
	//    example an octree. However, we have to be able to change the octree if we change the robot's position (e.g.
	//    remove the object from it's old position in the octree and readd it using it's new position). As far as I've
	//    seen, our Octree class does not yet support such methods.
	BOOST_FOREACH(const RobotPtr& other_robot, robots) {
		if (other_robot.get() == &robot)
			continue; // skip 'self-collision'
		const double distance = boost::numeric::ublas::norm_2(other_robot->position() - robot.position());
		if (distance < clearance_)
			return other_robot; // found collision
	}
	return RobotPtr(); // no collision found
}

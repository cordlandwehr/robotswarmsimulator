/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  collision_position_request_handler.cc
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 31.03.09.
 *  Copyright 2009 Peter Kling. All rights reserved.
 */

#include <EventHandlers/collision_position_request_handler.h>

#include <algorithm>

#include <boost/foreach.hpp>

#include <Model/robot_data.h>
#include <Model/world_information.h>
#include <Requests/position_request.h>
#include <Utilities/console_output.h>
#include <Views/octree_utilities.h>

CollisionPositionRequestHandler::CollisionPositionRequestHandler(
    CollisionStrategy strategy, double clearance,
    unsigned int seed, double discard_probability,
    const History& history) :
        VectorRequestHandler(seed, discard_probability, history),
        strategy_(strategy),
        clearance_(clearance),
        collision_tree_time_(-1) {}


bool CollisionPositionRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {
	// this handler does support only position requests
	typedef boost::shared_ptr<const PositionRequest> ConstPositionRequestPtr;
	ConstPositionRequestPtr position_request =
	    boost::dynamic_pointer_cast<const PositionRequest>(request);
	if(!position_request)
		throw std::invalid_argument("Not a position request.");

	// get requesting robot
	const boost::shared_ptr<RobotIdentifier>& robot_id =
	    position_request->robot().id();
	RobotData& robot = world_information->get_according_robot_data(robot_id);

	// check if this request is part of a new world_information
	// (if so, we need to rebuild the collision tree)
	if (collision_tree_time_ < world_information->time())
		update_collision_tree(*world_information);

	// Remove requesting robot from collision tree
	// (will be readded at the end of this method).
	// This is important: the robot will probably be moved by the following
	// commands and thus, the collision tree would become invalid
	// (the robot may be placed in the wrong octree node)
	collision_tree_->remove_robot(
	    world_information->get_according_robot_data_ptr(robot_id));

	// if robot is already collided, we abort
	// (no good way to resolve such collisions)
	if (find_colliding_robot(robot, *world_information))
		return false;

	// two boolean variables used to propagate information about the
	// request handling process to caller of this method
	bool handle_result;
	bool collision_result = false;

	// handle request, but save robot's current position
	// (handling itself is done by super class)
	const Vector3d old_position = robot.position();
	handle_result =
	    VectorRequestHandler::handle_request_reliable(world_information,
	                                                  request);

	// collision handling
	if (strategy_ == STOP) {
		// STOP strategy: reject request on collision
		// (i.e. undo the already applied request)
		if (find_colliding_robot(robot, *world_information)) {
			boost::shared_ptr<Vector3d> old_position_ptr(
			    new Vector3d(old_position));
			robot.set_position(old_position_ptr);
			collision_result = true;
		}
	} else if (strategy_ == TOUCH) {
		// TOUCH strategy: on collision, move back until colliding robots are touching
		while (const RobotPtr& other_robot =
		       find_colliding_robot(robot, *world_information)) {
			move_back_to_touchpoint(robot, *other_robot, old_position);
			collision_result = true;
		}
	}

	// debug output if a collision happened
	if (collision_result) {
		ConsoleOutput::log(ConsoleOutput::EventHandler, ConsoleOutput::debug)
			<< "Detected collision at time "
		    << world_information->time()
		    << std::endl;
	}

	// re-add current robot to the collision tree
	collision_tree_->add_robot(
	    world_information->get_according_robot_data_ptr(robot_id));

	// return whether we could perform the request exactly as requested
	return !collision_result && handle_result;
}


// This method essentially solves the quadratic equation for the value alpha
// that indicates how far the given robot must be moved backwards towards
// it's old_position such that it has distance clearance_ to the other_robot.
// Note that the resulting alpha should always lie in the closed interval [0,1]
void CollisionPositionRequestHandler::move_back_to_touchpoint(
    RobotData& robot,
    const RobotData& other_robot,
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

	// The result of the general solution term for quadratic equations
	// (splitted in the two summands; note that we need
	// only the 'quadr_sol_left + quadr_sol_right' solution and not the
	// 'quadr_sol_left - quadr_sol_right' (which is negative).
	const double quadr_sol_left = vector_product/new_to_old_dist_sqr;
	const double quadr_sol_right =
	    std::sqrt(vector_product*vector_product
	              + clearance_*clearance_ * new_to_old_dist_sqr
	              - new_to_other_dist_sqr * new_to_old_dist_sqr)
	    / new_to_old_dist_sqr;
	const double quadr_solution = quadr_sol_left + quadr_sol_right;

	// Due to rounding errors, we have to be somewhat tollerant. More exactly:
	//     - make sure that the solution is <= 1
	//       (if violated, then due to rounding errors!)
	//     - we allow/force an (very small) epsilon gap between two objects,
	//       to make sure that these objects are not
	//       considered as colliding again (would cause a livelock)

	// force a gap of (maximum) size 10^(-12) between the two objects
	const double epsilon = 1E-12;
	const double alpha = std::min(1., quadr_solution + epsilon/new_to_old_dist);

	// place robot at corresponding position
	boost::shared_ptr<Vector3d> corrected_position(
	    new Vector3d(robot.position()+alpha*new_to_old));
	robot.set_position(corrected_position);
}

CollisionPositionRequestHandler::RobotPtr CollisionPositionRequestHandler::find_colliding_robot(
    const RobotData& robot,
    WorldInformation& world_information) {
	// use the octree to find the colliding robots
	std::vector<boost::shared_ptr<RobotIdentifier> > colliding_robots;
	colliding_robots =
	    OctreeUtilities::get_visible_robots_by_radius(collision_tree_,
	                                                  robot.position(),
	                                                  clearance_,
	                                                  robot);

	// return the first colliding robot if we found one; otherwise,
	// return an empty pointer
	if (!colliding_robots.empty())
		return world_information.get_according_robot_data_ptr(
		    colliding_robots[0]);
	return RobotPtr();
}


void CollisionPositionRequestHandler::update_collision_tree(
    const WorldInformation& world_information) {
	// empty, because markers can not collide
	std::vector<boost::shared_ptr<WorldObject> > markers;
	// empty (obstacle collisions not yet supported)
	std::vector<boost::shared_ptr<Obstacle> > obstacles;

	// creates the collision tree
	if (!collision_tree_)
		collision_tree_.reset(new Octree(50, clearance_));
	collision_tree_->create_tree(markers, obstacles,
	                             world_information.robot_data());

	// update collision tree's creation time
	collision_tree_time_ = world_information.time();
}

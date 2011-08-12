/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 *  pull_spin_robot.cc
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 18.04.09.
 *  Copyright 2009 Peter Kling. All rights reserved.
 */

#include <vector>

#include <boost/foreach.hpp>

#include <ComputationalGeometry/point_algorithms.h>

#include <Model/robot_identifier.h>

#include <Requests/position_request.h>

#include <RobotImplementations/pull_spin_robot.h>

#include <Views/view.h>


namespace { // some internals
	boost::scoped_ptr<DistributionGenerator> prg;
	const double kEpsilon = 0.0000000001; ///< Tollerance for numeric errors.
}


PullSpinRobot::PullSpinRobot(boost::shared_ptr<RobotIdentifier> id)
: Robot(id), current_phase_(PULL), step_length_(0.2), spin_length_(3.1415/2.), spin_steps_nr_(spin_length_/step_length_),
  spin_counter_(0) { 
	if (!prg) { // all robots use the same prg, so we initialize it only once
		prg.reset(new DistributionGenerator(89713));
		prg->init_uniform_real(0., 1.);
		prg->init_uniform_on_sphere(3);
	}
}


PullSpinRobot::ResultContainer PullSpinRobot::compute() {	
	switch(current_phase_) {
		case(PULL):
			return pull();
			break;
			
		case(SPIN):
			 return spin();
			 break;
			 
		default:
			assert(false); // this should never happen
			return ResultContainer();
			break;
	}
}


const PullSpinRobot::ResultContainer PullSpinRobot::pull() {
	// compute the cog of all visible robots
	const Vector3d cog = compute_cog();
	
	// compute movement vector
	const double distance = boost::numeric::ublas::norm_2(cog);
	if (distance <= 0) // we're already at the COG --> nothing to do
		return ResultContainer();
	const boost::shared_ptr<Vector3d> movement_vector(new Vector3d(cog * step_length_/distance));
	
	// should we switch into the SPIN phase?
	if (prg->get_value_uniform_real() > 0.9)
		current_phase_ = SPIN;
	
	// set up and return request
	boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, movement_vector));
	ResultContainer results;
	results.insert(request);
	return results;
}


const PullSpinRobot::ResultContainer PullSpinRobot::spin() {
	if (spin_counter_ == 0) { // beginning of a new SPIN phase
		// compute movement circle
		movement_circle_center_ = compute_cog();
		movement_circle_base1_  = -movement_circle_center_;
		movement_circle_base2_  = compute_random_orthogonal_vector(movement_circle_base1_);
		movement_circle_radius_ = boost::numeric::ublas::norm_2(movement_circle_center_);
	}
	
	spin_counter_++; // proceed SPIN phase
	
	// compute movement vector
	const double alpha = spin_counter_*step_length_/movement_circle_radius_;
	const boost::shared_ptr<Vector3d> movement_vector(new Vector3d(
		movement_circle_center_ + cos(alpha) * movement_circle_base1_ + sin(alpha) * movement_circle_base2_)
	);

	// we have to update movement circle by translating it
	// TODO(peter): This assumes that every position request in the SPIN phase is fullfilled by the position request
	//     handler. This may be a problem under the presence of collisions, but then, in most cases all remaining
	//     position requests in the current SPIN phase will have no actual effect anyway. To cope with this situation
	//     correctly, we must have a possibility to actually detect a collision.
	movement_circle_center_ -= *movement_vector;
	
	// check for the end of this SPIN phase
	if (spin_counter_ >= spin_steps_nr_) {
		spin_counter_ = 0;
		current_phase_ = PULL;
	}
	
	// set up and return request
	boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, movement_vector));
	ResultContainer results;
	results.insert(request);
	return results;
}


const Vector3d PullSpinRobot::compute_cog() {
	typedef boost::shared_ptr<RobotIdentifier> RobotIDPtr;
	
	// compute a vector containing all visible robots' positions (including our own position)
	std::vector<RobotIDPtr> visible_robots = view_->get_visible_robots(*this);
	std::vector<Vector3d> robot_positions;
	robot_positions.push_back(view_->get_position(*this, id()));
	BOOST_FOREACH(const RobotIDPtr& robot, visible_robots) {
		robot_positions.push_back(view_->get_position(*this, robot));
	}
	
	// return the cog of all visible robots
	return PointAlgorithms::compute_COG(robot_positions);
}


const Vector3d PullSpinRobot::compute_random_orthogonal_vector(const Vector3d& v) {
	const double v_length = boost::numeric::ublas::norm_2(v);
	assert(v_length > kEpsilon); // this method needs a non-zero input vector
	
	// this computes a random, uniformly distributed, to v orthogonal vector having the same length as v
	Vector3d w;
	double w_length;
	do {
		w = prg->get_value_uniform_on_sphere_3d();                            // choose random vector in the unit sphere
		w -= boost::numeric::ublas::inner_prod(v, w)*v / (v_length*v_length); // project w onto plane perpendicular to v
		w_length = boost::numeric::ublas::norm_2(w);
	} while(w_length <= kEpsilon); // if this is zero, the random vector was collinear to v (or zero)
	w *= v_length/w_length;	// normalize w to same length as v
	return w;
}

/**
 * \file pull_spin_robot.h
 * \author Peter Kling
 * \date 18.04.09
 *  
 * Copyright 2009 Peter Kling. All rights reserved.
 */

#ifndef PULL_SPIN_ROBOT_H
#define PULL_SPIN_ROBOT_H

#include <Model/robot.h>

#include <Utilities/distribution_generator.h>


class PullSpinRobot : public Robot {	
public:
	typedef std::set<boost::shared_ptr<Request> > ResultContainer; ///< Type to hold the robot's computation results.
	
	enum RobotState {PULL, SPIN}; ///< Type for the different computation phases.
	
	
	PullSpinRobot(boost::shared_ptr<RobotIdentifier> id); ///< Constructs a PullSpinRobot with the given ID.
	
	
	std::string get_algorithm_id() const { return "PullSpinRobot"; } ///< Return the robot's identifier string.
	
	
	/**
	 * This robot type will compute the COG of all robots and move in that direction (PULL operation). If a collision
	 * happens in this phase, the robot stops moving towards the COG and instead starts a SPIN operation. In this
	 * phase, a robot chooses a random circular path on the sphere around the current COG with radius dist(robot, COG)
	 * and moves a specific distance on this path. Afterwards, another PULL operation is started.
	 */
	ResultContainer compute();
	
	
private:
	const ResultContainer pull(); ///< Executed in the robot's PULL phase.
	const ResultContainer spin(); ///< Executed in the robot's SPIN phase.
	
	const Vector3d compute_cog(); ///< Computes the COG for this and all visible robots.
	
	/**
	 * Computes a random, uniformly distributed, to v orthogonal vector having the same length as v. Note that the input
	 * vector has to be a non-zero vector.
	 */
	const Vector3d compute_random_orthogonal_vector(const Vector3d& v);
	
	
	RobotState current_phase_; ///< Indicates the current computation phase of this robot.
	const double step_length_; ///< Maximal step length a robot can do in one position request.
	
	Vector3d movement_circle_center_; ///< The center of the robot's movement circle for the SPIN phase.
	Vector3d movement_circle_base1_;  ///< First base vector of robot's movement circle for the SPIN phase.
	Vector3d movement_circle_base2_;  ///< Second base vector of robot's movement circle for the SPIN phase.
	double movement_circle_radius_;   ///< The radius of the robot's movement circle for the SPIN phase.
	
	const double spin_length_;         ///< Every robot spins this distance during the SPIN phase.
	const unsigned int spin_steps_nr_; ///< A spin phase has a duration of this many steps (=spin_length_/step_length_).
	unsigned int spin_counter_;        ///< Counts the number of computation steps done in the current SPIN phase.
};

#endif /* PULL_SPIN_ROBOT_H */

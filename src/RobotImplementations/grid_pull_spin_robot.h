/**
 * \file grid_pull_spin_robot.h
 * \author Peter Kling
 * \date 05.09.09
 *  
 * Copyright 2009 Peter Kling. All rights reserved.
 */

#ifndef GRID_PULL_SPIN_ROBOT_H
#define GRID_PULL_SPIN_ROBOT_H

#include <Model/robot.h>


/**
 * A fast implementation of the GridPullSpin algorithm. This algorithm assumes that the robots have global view. The
 * global view is used to compute an array that allows fast checks whether a given grid position is occupied or not.
 */
class GridPullSpinRobot : public Robot {	
public:
	typedef std::set<boost::shared_ptr<Request> > ResultContainer; ///< Type to hold the robot's computation results.
    typedef boost::shared_ptr<ResultContainer>    ResultContainerPtr;
    typedef boost::shared_ptr<PositionRequest>    PositionRequestPtr;
	
    
	GridPullSpinRobot(boost::shared_ptr<RobotIdentifier> id); ///< Constructs a GridPullSpinRobot with the given ID.
	
	
	std::string get_algorithm_id() const { return "GridPullSpinRobot"; } ///< Return the robot's identifier string.
	
	
	/**
	 * Standard GridPullSpin algorithm for the two dimensional case. The target point is given by the origin and robots
     * spin counterclockwise around it when they're not able to pull.
	 */
	ResultContainer compute();

private:
    const ResultContainerPtr x_pull(const int x, const int y);
    const ResultContainerPtr y_pull(const int x, const int y);
    const ResultContainerPtr spin(const int x, const int y);
    
    const bool position_occupied(const int x, const int y);
    void set_position_occupied(const int x, const int y, const bool value);
    
    const bool can_x_pull(const int x, const int y);
    
    const bool no_pull_at(const int x, const int y);
    
    const void update_robot_field();
    
    
    static int current_time_;
    static std::vector<bool> robot_field_;
    static unsigned int robot_field_x_dim_size_;
    static unsigned int robot_field_y_dim_size_;
    static int robot_field_min_x_;
    static int robot_field_min_y_;
    static int robot_field_max_x_;
    static int robot_field_max_y_;
};

#endif /* GRID_PULL_SPIN_ROBOT_H */

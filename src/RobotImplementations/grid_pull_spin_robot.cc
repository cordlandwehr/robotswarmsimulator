/*
 *  grid_pull_spin_robot.cc
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 05.09.09.
 *  Copyright 2009 Peter Kling. All rights reserved.
 */

#include <cstring>
#include <vector>

#include <boost/foreach.hpp>

#include <Model/robot_identifier.h>
#include <Requests/position_request.h>
#include <RobotImplementations/grid_pull_spin_robot.h>
#include <Views/view.h>


// initialization of static class elements
int GridPullSpinRobot::current_time_ = -1;
std::vector<bool> GridPullSpinRobot::robot_field_ = std::vector<bool>();
unsigned int GridPullSpinRobot::robot_field_x_dim_size_ = 0;
unsigned int GridPullSpinRobot::robot_field_y_dim_size_ = 0;
int GridPullSpinRobot::robot_field_min_x_ = 0;
int GridPullSpinRobot::robot_field_min_y_ = 0;
int GridPullSpinRobot::robot_field_max_x_ = 0;
int GridPullSpinRobot::robot_field_max_y_ = 0;


GridPullSpinRobot::GridPullSpinRobot(boost::shared_ptr<RobotIdentifier> id)
: Robot(id) {
}


GridPullSpinRobot::ResultContainer GridPullSpinRobot::compute() {
    // update robot field if not yet happend for this time step
    if (current_time_ != view_->get_time()) {
        current_time_ = view_->get_time();
        update_robot_field();
    }
    
    const Vector3d position = view_->get_robot_coordinate_system_origin(*this, id());
    const int x = static_cast<int>(position(0));
    const int y = static_cast<int>(position(1));
    
    ResultContainerPtr results;
    if (results = x_pull(x, y))
        return *results;
    if (results = y_pull(x, y))
        return *results;
    if (results = spin(x, y))
        return *results;
    return ResultContainer();
}


const GridPullSpinRobot::ResultContainerPtr GridPullSpinRobot::x_pull(const int x, const int y) {
    if (x == 0)
        return ResultContainerPtr();
    
    const int x_direction = x > 0 ? -1 : 1;
    if (!position_occupied(x+x_direction, y)) {
        const boost::shared_ptr<Vector3d> movement_vector(new Vector3d);
        (*movement_vector)(0) = x_direction;
        (*movement_vector)(1) = 0;
        (*movement_vector)(2) = 0;
        ResultContainerPtr results(new ResultContainer);
        PositionRequestPtr request(new PositionRequest(*this, movement_vector));
        results->insert(request);
        return results;
    }
    
    return ResultContainerPtr();
}

const GridPullSpinRobot::ResultContainerPtr GridPullSpinRobot::y_pull(const int x, const int y) {
    if (y == 0)
        return ResultContainerPtr();
    
    const int y_direction = y > 0 ? -1 : 1;
    if (!position_occupied(x, y+y_direction)) {
        const boost::shared_ptr<Vector3d> movement_vector(new Vector3d);
        (*movement_vector)(0) = 0;
        (*movement_vector)(1) = y_direction;
        (*movement_vector)(2) = 0;
        ResultContainerPtr results(new ResultContainer);
        PositionRequestPtr request(new PositionRequest(*this, movement_vector));
        results->insert(request);
        return results;
    }
    
    return ResultContainerPtr();
}


const GridPullSpinRobot::ResultContainerPtr GridPullSpinRobot::spin(const int x, const int y) {
    if (x == 0 && y == 0)
        return ResultContainerPtr();
    
    int x_direction;
    int y_direction;
    if (x == 0) {
        x_direction = y > 0 ? -1 : 1;
        y_direction = y > 0 ? -1 : 1;
    } else if (y == 0) {
        x_direction = x > 0 ? -1 :  1;
        y_direction = x > 0 ?  1 : -1;
    } else {
        x_direction = y > 0 ? -1 :  1;
        y_direction = x > 0 ?  1 : -1;
    }

    if (!position_occupied(x+x_direction, y+y_direction) && no_pull_at(x+x_direction, y+y_direction)) {
        const boost::shared_ptr<Vector3d> movement_vector(new Vector3d);
        (*movement_vector)(0) = x_direction;
        (*movement_vector)(1) = y_direction;
        (*movement_vector)(2) = 0;
        ResultContainerPtr results(new ResultContainer);
        PositionRequestPtr request(new PositionRequest(*this, movement_vector));
        results->insert(request);
        return results;
    }

    return ResultContainerPtr();
}


const bool GridPullSpinRobot::can_x_pull(const int x, const int y) {
    if (x == 0)
        return false;
    const int x_direction = x > 0 ? -1 : 1;
    if (!position_occupied(x+x_direction, y))
        return true;
    return false;
}

const bool GridPullSpinRobot::no_pull_at(const int x, const int y) {
    if (x == 0) {
        if (position_occupied(1, y) || position_occupied(-1, y))
            return false;
    } else {
        const int x_direction = x > 0 ? -1 : 1;
        if (position_occupied(x-x_direction, y))
            return false;
    }
    
    if (y == 0) {
        if ((position_occupied(x,  1) && !can_x_pull(x,  1)) || (position_occupied(x, -1) && !can_x_pull(x, -1)))
            return false;
    } else {
        const int y_direction = y > 0 ? -1 : 1;
        if (position_occupied(x, y-y_direction) && !can_x_pull(x, y-y_direction))
            return false;
    }
    
    return true;
}


const bool GridPullSpinRobot::position_occupied(const int x, const int y) {
    unsigned int x_transl = static_cast<unsigned int>(x - robot_field_min_x_);
    unsigned int y_transl = static_cast<unsigned int>(y - robot_field_min_y_);
    return robot_field_[y_transl*robot_field_x_dim_size_ + x_transl];
    
//  // Alternative approach for checking possition occupation. Uses less memory but is computationally more costly.
//  // To enable, uncomment the lines below, comment out the ones above and make sure that update_robot_field() is not
//  // called by the compute() method.
//    const Vector3d& position = view_->get_robot_coordinate_system_origin(*this, id());
//    if (x == static_cast<int>(position(0)) && y == static_cast<int>(position(1)))
//        return true;
//    
//    typedef boost::shared_ptr<RobotIdentifier> RobotIDPtr;
//    std::vector<RobotIDPtr> robots = view_->get_visible_robots(*this);
//    BOOST_FOREACH(const RobotIDPtr& robot, robots) {
//        const Vector3d& robot_position = view_->get_robot_coordinate_system_origin(*this, robot);
//        if (x == static_cast<int>(robot_position(0)) && y == static_cast<int>(robot_position(1)))
//            return true;
//    }
//    return false;
}

void GridPullSpinRobot::set_position_occupied(const int x, const int y, const bool value) {
    unsigned int x_transl = static_cast<unsigned int>(x - robot_field_min_x_);
    unsigned int y_transl = static_cast<unsigned int>(y - robot_field_min_y_);
    robot_field_[y_transl*robot_field_x_dim_size_ + x_transl] = value;
}


const void GridPullSpinRobot::update_robot_field() {
    typedef boost::shared_ptr<RobotIdentifier> RobotIDPtr;
    
    const Vector3d position = view_->get_robot_coordinate_system_origin(*this, id());
    const int x = static_cast<int>(position(0));
    const int y = static_cast<int>(position(1));
    
    robot_field_min_x_ = x;
    robot_field_min_y_ = y;
    robot_field_max_x_ = x;
    robot_field_max_y_ = y;
    
    std::vector<RobotIDPtr> robots = view_->get_visible_robots(*this);
    BOOST_FOREACH(const RobotIDPtr& robot, robots) {
        const Vector3d& robot_position = view_->get_robot_coordinate_system_origin(*this, robot);
        const int robot_x = static_cast<int>(robot_position(0));
        const int robot_y = static_cast<int>(robot_position(1));
        if (robot_x < robot_field_min_x_)
            robot_field_min_x_ = robot_x;
        if (robot_x > robot_field_max_x_)
            robot_field_max_x_ = robot_x;
        if (robot_y < robot_field_min_y_)
            robot_field_min_y_ = robot_y;
        if (robot_y > robot_field_max_y_)
            robot_field_max_y_ = robot_y;
    }
    
    robot_field_min_x_--;
    robot_field_min_y_--;
    robot_field_max_x_++;
    robot_field_max_y_++;
    robot_field_x_dim_size_ = static_cast<unsigned int>(robot_field_max_x_ - robot_field_min_x_ + 1);
    robot_field_y_dim_size_ = static_cast<unsigned int>(robot_field_max_y_ - robot_field_min_y_ + 1);
    
    robot_field_ = std::vector<bool>(robot_field_x_dim_size_*robot_field_y_dim_size_, false);
    set_position_occupied(x, y, true);
    BOOST_FOREACH(const RobotIDPtr& robot, robots) {
        const Vector3d& robot_position = view_->get_robot_coordinate_system_origin(*this, robot);
        set_position_occupied(static_cast<int>(robot_position(0)), static_cast<int>(robot_position(1)), true);
    }
}

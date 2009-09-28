/**
 * \file collision_position_request_handler.h
 * \author Peter Kling
 * \date 31.03.09
 *
 * Copyright 2009 Peter Kling. All rights reserved.
 */

#ifndef COLLISION_POSITION_REQUEST_HANDLER_H
#define COLLISION_POSITION_REQUEST_HANDLER_H

#include <vector>

#include <EventHandlers/vector_request_handler.h>
#include <Views/octree.h>


/**
 * A position request handler with collision detection.
 *
 * It supports standard options provided by normale VectorRequestHandler
 * instances as for example unreliable event handling or vector modifiers.
 * In addition, it will detect collisions between the requesting and
 * any other object. If a collision is recognized, the handler will (depending
 * on it's configuration) discard the request completely or move the object as
 * far as possible without causing a collision
 * (i.e. the object will touch another object).
 *
 * Note that standard handling of request
 * (by use of VectorRequestHandler::handle_request_reliable()) is done
 * <em>prior</em> to collision handling. Therefore, even if the robot requested
 * a valid position, its request may be discarded due to a detected collision
 * caused by (the configured) inaccurateness.
 */
class CollisionPositionRequestHandler : public VectorRequestHandler {
public:
	typedef boost::shared_ptr<RobotData> RobotPtr;
	typedef std::vector<RobotPtr> RobotContainer;

	enum CollisionStrategy {
		STOP,  // < If a collision is detected, the request is discarded.
		TOUCH  // < If a collision is detected, the robot is moved
		       //   as far as possible in the requested direction.
	};


	CollisionPositionRequestHandler(CollisionStrategy strategy,
	                                double clearance,
	                                unsigned int seed,
	                                double discard_probability,
	                                const History& history);


protected:
	/**
	 * Handling of the position request.
	 *
	 * The request is actually handles be VectorRequestHandler::handle_request_reliable(). However, additionally we
	 * check for collisions and react according to the collision strategy.
	 *
	 * \return Returns true if the request has been performed as given and false if it was changed (by one of the vector
	 *         modifiers or due to a collision).
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);

private:
	/**
	 * Checks wether one of the robots in the given world is colliding with the given robot.
	 *
	 * If such a robot is found, a pointer to it is returned. If not,
	 * a NULL pointer is returned. Note 'self-collision' is not considered,
	 * that is, this will never return a pointer to the robot itself.
	 */
	RobotPtr find_colliding_robot(const RobotData& robot,
	                              WorldInformation& world_information);

	/**
	 * Moves the given robot back on the line between it's current position
	 * and the given old_position such that is no longer colliding but touching
	 * the given other robot. It is assumed that old_position is valid,
	 * i.e. collision free
	 * (the robot will <em>not</em> be moved farther back than this point).
	 *
	 * \param robot Robot that is to be moved back.
	 * \param other_robot Collision partner the robot is colliding with.
	 * \param old_position Original position of the robot
	 * (that is before the collision occured;
	 * this is assumed to be collision free.
	 */
	void move_back_to_touchpoint(RobotData& robot,
	                             const RobotData& other_robot,
	                             const Vector3d& old_position);

	/**
	 * Resets the collision tree to reflect the given world_information
	 * and updates the collision_tree_time_ to the
	 * time the given world_information represents.
	 */
	void update_collision_tree(const WorldInformation& world_information);

	// Indicates the strategy used if a collision is detected.
	const CollisionStrategy strategy_;
	// Two robots having a distance less than this value are considered collided.
	const double clearance_;

	// Used to hold the number of collision candidates low.
	boost::shared_ptr<Octree> collision_tree_;
	// Holds the time the current collision_tree_ was created for.
	int collision_tree_time_;
};

#endif /* COLLISION_POSITION_REQUEST_HANDLER_H */

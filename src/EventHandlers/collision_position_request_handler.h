/**
 * \file collision_position_request_handler.h
 * \author Peter Kling
 * \date 31.03.09
 *  
 * Copyright 2009 Peter Kling. All rights reserved.
 */
 
#ifndef COLLISION_POSITION_REQUEST_HANDLER_H
#define COLLISION_POSITION_REQUEST_HANDLER_H

#include <EventHandlers/vector_request_handler.h>


/**
 * A position request handler with collision detection.
 *
 * It supports standard options provided by normale VectorRequestHandler instances as for example unreliable event
 * handling or vector modifiers. In addition, it will detect collisions between the requesting and any other object. If
 * a collision is recognized, the handler will (depending on it's configuration) discard the request completely or move
 * the object as far as possible without causing a collision (i.e. the object will touch another object).
 *
 * Note that standard handling of request (by use of VectorRequestHandler::handle_request_reliable()) is done
 * <em>prior</em> to collision handling. Therefore, even if the robot requested a valid position, its request may be
 * discarded due to a detected collision caused by (the configured) inaccurateness.
 */
class CollisionPositionRequestHandler : public VectorRequestHandler {
public:
	typedef boost::shared_ptr<RobotData> RobotPtr;
	typedef std::vector<RobotPtr> RobotContainer;
	
	enum CollisionStrategy {
		STOP, ///< If a collision is detected, the request is discarded.
		TOUCH ///< If a collision is detected, the robot is moved as far as possible in the requested direction.
	};
	
	
	CollisionPositionRequestHandler(CollisionStrategy strategy, double clearance, unsigned int seed,
	                                double discard_probability, const History& history);
	
	
protected:
	/**
	 * Handling of the position request.
	 *
	 * The request is actually handles be VectorRequestHandler::handle_request_reliable(). However, additionally we
	 * check for collisions and react according to
	 */
	virtual void handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
	                                     boost::shared_ptr<const Request> request);
										 
	
private:	
	/**
	 * Checks wether one of the robots in the given container is colliding with the given robot.
	 *
	 * If such a robot is found, a pointer to it is returned. If not, a NULL pointer is returned. Note that the
	 * container may contain the regarded robot itself; however, 'self-collision' is not considered, that is, this will
	 * never return a pointer to the robot itself.
	 */
	RobotPtr find_colliding_robot(const RobotData& robot, const RobotContainer& robots);
	
	
	/**
	 * Moves the given robot back on the line between it's current position and the given old_position such that is no
	 * longer colliding but touching the given other robot. It is assumed that old_position is valid, i.e. collision
	 * free (the robot will <em>not</em> be moved farther back than this point).
	 *
	 * \param robot Robot that is to be moved back.
	 * \param other_robot Collision partner the robot is colliding with.
	 * \param old_position Original position of the robot (that is before the collision occured; this is assumed to be
	 *        collision free.
	 */
	void move_back_to_touchpoint(RobotData& robot, const RobotData& other_robot, const Vector3d& old_position);
	
	
	const CollisionStrategy strategy_; ///< Indicates the strategy used if a collision is detected.
	const double clearance_; ///< Two robots having a distance less than this value are considered collided.
};

#endif /* COLLISION_POSITION_REQUEST_HANDLER_H */

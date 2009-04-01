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
 */
class CollisionPositionRequestHandler : public VectorRequestHandler {
public:
	enum CollsionStrategy {
		STOP, ///< If a collision is detected, the request is discarded.
		TOUCH ///< If a collision is detected, the robot is moved as far as possible in the requested direction.
	}
	
	
	CollisionPositionRequestHandler(CollisionStrategy strategy, unsigned int seed, double discard_probability,
	                                const History& history)
	: VectorRequestHandler(seed, discard_probability, history), strategy_(strategy) { }
	
	
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
	const CollisionStrategy strategy_; ///< Indicates the strategy used if a collision is detected.
};


#endif /* COLLISION_POSITION_REQUEST_HANDLER_H */

/*
 * event_handler.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include <boost/smart_ptr.hpp>

// forward declarations
class Event;
class ComputeEvent;
class LookEvent;
class HandleRequestsEvent;

class Request;
class AccelerationRequest;
class MarkerRequest;
class PositionRequest;
class TypeChangeRequest;
class VelocityRequest;

class WorldInformation;
class SimulationListener;
class History;

/**
 *
 *  * \brief The event handler determines, according to some user�specified rules,
 * how to apply the different requests to the world.
 *
 * The abstract event handler class provides the following functionality:
 * 1. handle compute and look event by delegation to RobotControl
 * 2. partly handle HandleRequests events by extrapolating the robot positions and calling the request handlers
 * 3. registering of simulation listeners and updating them when the world changes
 *
 * In most cases a subclass of EventHandler should only define custom handle_request functions for the
 * possible requests
 */
class EventHandler {
public:
	EventHandler(boost::shared_ptr<History> history):history_(history) {}
	virtual ~EventHandler() {}

	/**
	 * handles the given event.
	 */
	void handle_event(boost::shared_ptr<Event> event);

	/**
	 * registers a new listener.
	 */
	void register_listener(boost::shared_ptr<SimulationListener> listener);

private:
	/**
	 * handles the given look event by delegating it to RobotControl
	 */
	void handle_look_event(boost::shared_ptr<LookEvent> look_event);

	/**
	 * handles the given compute event by delegating it to RobotControl
	 */
	void handle_compute_event(boost::shared_ptr<ComputeEvent> compute_event);

	/**
	 * handles the given HandleRequests event by doing the following
	 * 1. producing a new WorldInformation object by extrapolating and handling requests
	 * 2. adding the new WorldInformation object to the history
	 * 3. informing all listeners of the changed world state
	 */
	void handle_requests_event(boost::shared_ptr<HandleRequestsEvent> handle_requests_event);

	/**
	 * handles the given request by delegating to one of the custom handler methods
	 */
	void handle_request(boost::shared_ptr<Request> request);

	/**
	 * virtual method for handling acceleration requests
	 */
	virtual void handle_acceleration_request(boost::shared_ptr<AccelerationRequest> acceleration_request) = 0;

	/**
	 * virtual method for handling marker requests
	 */
	virtual void handle_marker_request(boost::shared_ptr<MarkerRequest> marker_request) = 0;

	/**
	 * virtual method for handling position requests
	 */
	virtual void handle_position_request(boost::shared_ptr<PositionRequest> position_request) = 0;

	/**
	 * virtual method for handling type change requests
	 */
	virtual void handle_type_change_request(boost::shared_ptr<TypeChangeRequest> type_change_request) = 0;

	/**
	 * virtual method for handling velocity requests
	 */
	virtual void handle_velocity_request(boost::shared_ptr<VelocityRequest> velocity_request) = 0;

	/**
	 * generates a new WorldInformation object by extrapolating and handling requests
	 */
	boost::shared_ptr<WorldInformation> produce_world_information();

	/**
	 * generates a new WorldInformation object by extrapolating it from the newest old one to time t
	 */
	boost::shared_ptr<WorldInformation> extrapolate_old_world_information(int time);

	/**
	 * informs all listeners of the changed world state
	 */
	void update_listeners();


	boost::shared_ptr<History> history_;
	vector<boost::shared_ptr<SimulationListener> > listeners_;
};

#endif /* EVENT_HANDLER_H_ */
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
class RobotControl;
class History;

/**
 *
 * \brief The event handler determines, according to some user–specified rules,
 * how to apply the different requests to the world.
 *
 * The abstract event handler class provides the following functionality:
 * 1. handle compute and look event by delegation to RobotControl
 * 2. partly handle HandleRequests events by extrapolating the robot positions and calling the request handlers
 * 3. registering of simulation listeners and updating them when the world changes
 *
 * In most cases a subclass of EventHandler should only define custom handle_request functions for the
 * possible requests. If it does not provide a custom implementation for a particular handle_*_request method, a warning
 * message is issued upon the occurrence of such a request.
 */
class EventHandler {
protected:
	/**
	 * Protected constructor, to mark class as abstract.
	 */
	EventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control): history_(history),
	                                                                                                 robot_control_(robot_control),
	                                                                                                 time_of_last_event_(0) {}

public:
	virtual ~EventHandler() {}

	/**
	 * handles the given event. By calling appropriate handlers and updating the listeners.
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
	 */
	void handle_handle_requests_event(boost::shared_ptr<HandleRequestsEvent> handle_requests_event);

	/**
	 * virtual method for handling acceleration requests
	 * TODO(peter) Will we provide some kind of logging facility to be used by the whole project? If so, adopt this
	 * method (and all other handle_*_request methods).
	 */
	virtual void handle_acceleration_request(boost::shared_ptr<WorldInformation> world_information,
	                                         boost::shared_ptr<const AccelerationRequest> acceleration_request) {
		std::cerr << "Warning: AccelerationRequest not supported" << std::endl;
	}

	/**
	 * virtual method for handling marker requests
	 */
	virtual void handle_marker_request(boost::shared_ptr<WorldInformation> world_information,
	                                   boost::shared_ptr<const MarkerRequest> marker_request) {
		std::cerr << "Warning: MarkerRequest not supported" << std::endl;
	}

	/**
	 * virtual method for handling position requests
	 */
	virtual void handle_position_request(boost::shared_ptr<WorldInformation> world_information,
	                                     boost::shared_ptr<const PositionRequest> position_request) {
		std::cerr << "Warning: PositionRequest not supported" << std::endl;
	}

	/**
	 * virtual method for handling type change requests
	 */
	virtual void handle_type_change_request(boost::shared_ptr<WorldInformation> world_information,
	                                        boost::shared_ptr<const TypeChangeRequest> type_change_request) {
		std::cerr << "Warning: TypeChangeRequest not supported" << std::endl;
	}

	/**
	 * virtual method for handling velocity requests
	 */
	virtual void handle_velocity_request(boost::shared_ptr<WorldInformation> world_information,
	                                     boost::shared_ptr<const VelocityRequest> velocity_request) {
		std::cerr << "Warning: VelocityRequest not supported" << std::endl;
	}

	/**
	 * informs all listeners after each event
	 */
	void update_listeners(boost::shared_ptr<Event> event);

	/**
	 * generates a new WorldInformation object by extrapolating it from the newest old one to time t
	 */
	boost::shared_ptr<WorldInformation> extrapolate_old_world_information(int time);


	vector<boost::shared_ptr<SimulationListener> > listeners_;
	boost::shared_ptr<History> history_;
	boost::shared_ptr<RobotControl> robot_control_;

	/**
	 * The time the last event has happened. Used to ensure correct ordering of events.
	 */
	int time_of_last_event_;
};

#endif /* EVENT_HANDLER_H_ */

/*
 * event_handler.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include <vector>

#include <boost/smart_ptr.hpp>

#include "../EventHandlers/vector_request_handler.h"
#include "../EventHandlers/marker_request_handler.h"
#include "../EventHandlers/type_change_request_handler.h"
#include "../EventHandlers/marker_change_request_handler.h"
#include "../EventHandlers/color_change_request_handler.h"
#include "../EventHandlers/message_request_handler.h"
#include "../EventHandlers/insert_edge_request_handler.h"
#include "../EventHandlers/remove_edge_request_handler.h"

#include "../Events/handle_requests_event.h"

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
class MarkerChangeRequest;
class ColorChangeRequest;

class WorldInformation;
class SimulationListener;
class RobotControl;
class History;

/**
 *
 * \brief The event handler determines, according to some user�specified rules,
 * how to apply the different requests to the world.
 *
 * The EventHandler class provides the following functionality:
 * 1. handle compute and look event by delegation to RobotControl
 * 2. partly handle HandleRequests events by extrapolating the robot positions and calling the RequestHandlers
 * 3. registering of simulation listeners and updating them when the world changes
 *
 * EventHandler should be provided with custom RequestHandlers for the
 * possible requests. If there is no RequestHandler for a particular type of request, a warning
 * message is issued upon the occurrence of such a request.
 */
class EventHandler {
	friend class EventHandlerFactoryTest;
	friend class LoadMainProjectFileTest;

public:
	EventHandler(boost::shared_ptr<History> history,
	             boost::shared_ptr<RobotControl> robot_control) :
	    history_(history),
	    robot_control_(robot_control),
	    time_of_last_event_(0) {}

	/**
	 * handles the given event. By calling appropriate handlers and updating the listeners.
	 */
	void handle_event(boost::shared_ptr<Event> event, TimePoint& time_point);

	/**
	 * registers a new listener.
	 */
	void register_listener(boost::shared_ptr<SimulationListener> listener);

	/**
	 * setter for position request handler
	 */
	void set_position_request_handler(
	    boost::shared_ptr<VectorRequestHandler> position_request_handler) {
		position_request_handler_ = position_request_handler;
	}

	/**
	 * setter for velocity request handler
	 */
	void set_velocity_request_handler(
	    boost::shared_ptr<VectorRequestHandler> velocity_request_handler) {
		velocity_request_handler_ = velocity_request_handler;
	}

	/**
	 * setter for acceleration request handler
	 */
	void set_acceleration_request_handler(
	    boost::shared_ptr<VectorRequestHandler> acceleration_request_handler) {
		acceleration_request_handler_ = acceleration_request_handler;
	}

	/**
	 * setter for marker request handler
	 */
	void set_marker_request_handler(
	    boost::shared_ptr<MarkerRequestHandler> marker_request_handler) {
		marker_request_handler_ = marker_request_handler;
	}

	/**
	 * setter for type change request handler
	 */
	void set_type_change_request_handler(
	    boost::shared_ptr<TypeChangeRequestHandler> request_handler) {
		type_change_request_handler_ = request_handler;
	}

	void set_marker_change_request_handler(
	    boost::shared_ptr<MarkerChangeRequestHandler> request_handler) {
		marker_change_request_handler_ = request_handler;
	}

	void set_color_change_request_handler(
	    boost::shared_ptr<ColorChangeRequestHandler> request_handler) {
		color_change_request_handler_ = request_handler;
	}

	void set_message_request_handler(
		boost::shared_ptr<MessageRequestHandler> request_handler) {
		message_request_handler_ = request_handler;
	}

	void set_insert_edge_request_handler(
		boost::shared_ptr<InsertEdgeRequestHandler> request_handler) {
		insert_edge_request_handler_ = request_handler;
	}

	void set_remove_edge_request_handler(
		boost::shared_ptr<RemoveEdgeRequestHandler> request_handler) {
		remove_edge_request_handler_ = request_handler;
	}

private:
	/**
	 * handles the given look event by delegating it to RobotControl
	 */
	boost::shared_ptr<WorldInformation> handle_look_event(
	    boost::shared_ptr<LookEvent> look_event);

	/**
	 * handles the given compute event by delegating it to RobotControl
	 */
	boost::shared_ptr<WorldInformation> handle_compute_event(
	    boost::shared_ptr<ComputeEvent> compute_event);

	/**
	 * handles the given HandleRequests event by doing the following
	 * 1. producing a new WorldInformation object by extrapolating and
	 * handling requests
	 * 2. adding the new WorldInformation object to the history
	 */
	boost::shared_ptr<WorldInformation> handle_handle_requests_event(
	    boost::shared_ptr<HandleRequestsEvent> handle_requests_event);

	/**
	 * informs all listeners after each event
	 */
	void update_listeners(TimePoint& time_point, boost::shared_ptr<Event> event);

	/**
	 * generates a new WorldInformation object by extrapolating it
	 * from the newest old one to time t
	 */
	boost::shared_ptr<WorldInformation> extrapolate_old_world_information(
	    int time);

	/**
	 * Request Handlers. One for each type of request.
	 */
	boost::shared_ptr<VectorRequestHandler> position_request_handler_;
	boost::shared_ptr<VectorRequestHandler> velocity_request_handler_;
	boost::shared_ptr<VectorRequestHandler> acceleration_request_handler_;
	boost::shared_ptr<MarkerRequestHandler> marker_request_handler_;
	boost::shared_ptr<TypeChangeRequestHandler> type_change_request_handler_;
	boost::shared_ptr<MarkerChangeRequestHandler> marker_change_request_handler_;
	boost::shared_ptr<ColorChangeRequestHandler> color_change_request_handler_;
	boost::shared_ptr<MessageRequestHandler> message_request_handler_;
	boost::shared_ptr<InsertEdgeRequestHandler> insert_edge_request_handler_;
	boost::shared_ptr<RemoveEdgeRequestHandler> remove_edge_request_handler_;

	std::vector<boost::shared_ptr<SimulationListener> > listeners_;
	boost::shared_ptr<History> history_;
	boost::shared_ptr<RobotControl> robot_control_;

	/**
	 * The time the last event has happened. Used to ensure correct ordering of events.
	 */
	int time_of_last_event_;
};

#endif /* EVENT_HANDLER_H_ */

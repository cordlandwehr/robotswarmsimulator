/*
 * exact_marker_event_handler.h
 *
 *  Created on: Jan 20, 2009
 *      Author: craupach
 */

#ifndef EXACT_MARKER_EVENT_HANDLER_H_
#define EXACT_MARKER_EVENT_HANDLER_H_


#include <boost/smart_ptr.hpp>

#include "event_handler.h"


// forward declarations
class History;
class MarkerRequest;
class RobotControl;
class WorldInformation;


/**
 * An event handler supporting only MarkerRequests.
 */
class ExactMarkerEventHandler : public EventHandler {
public:
	ExactMarkerEventHandler(boost::shared_ptr<History> history, boost::shared_ptr<RobotControl> robot_control)
		: EventHandler(history, robot_control) { };

protected:
	/**
	 * \brief Provides default handling of marker requests.
	 * \param world_information WorldInformation object to be affected by the request
	 * \param marker_request marker request to handle
	 */
	void handle_marker_request(boost::shared_ptr<WorldInformation> world_information,
							   boost::shared_ptr<const MarkerRequest> marker_request) {
		EventHandler::handle_marker_request_default(world_information, marker_request);
	}
};

#endif /* EXACT_MARKER_EVENT_HANDLER_H_ */

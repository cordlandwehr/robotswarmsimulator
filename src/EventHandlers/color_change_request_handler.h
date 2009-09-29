/*
 * color_change_request_handler.h
 *
 *  Created on: 02.07.2009
 *      Author: kamil
 */

#ifndef COLOR_CHANGE_REQUEST_HANDLER_H_
#define COLOR_CHANGE_REQUEST_HANDLER_H_

#include "../EventHandlers/request_handler.h"

/**
 * \brief Handles MarkerChangeRequests by changing the specified marker as requested.
 */
class ColorChangeRequestHandler : public RequestHandler {
public:
	ColorChangeRequestHandler(unsigned int seed, double discard_probability,
	                          const History& history)
	: RequestHandler(seed, discard_probability, history) { }

	virtual ~ColorChangeRequestHandler() { }

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* COLOR_CHANGE_REQUEST_HANDLER_H_ */

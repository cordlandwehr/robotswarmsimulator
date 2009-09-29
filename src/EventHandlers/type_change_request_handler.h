/*
 * type_change_request_handler.h
 *
 *  Created on: Jan 31, 2009
 *      Author: craupach
 */

#ifndef TYPE_CHANGE_REQUEST_HANDLER_H_
#define TYPE_CHANGE_REQUEST_HANDLER_H_

#include "../EventHandlers/request_handler.h"

/**
 * Standard type change request handler. Fullfills every type change request.
 */
class TypeChangeRequestHandler : public RequestHandler {
	friend class LoadMainProjectFileTest;
public:
	TypeChangeRequestHandler(
	    unsigned int seed,
	    double discard_probability,
	    const History& history) : RequestHandler(seed,
	                                             discard_probability,
	                                             history) {}
    virtual ~TypeChangeRequestHandler() {}

protected:
	/**
	 * \brief Performs the given requested without any changes.
	 * \return Returns true.
	 */
	virtual bool handle_request_reliable(
	    boost::shared_ptr<WorldInformation> world_information,
	    boost::shared_ptr<const Request> request);
};

#endif /* TYPE_CHANGE_REQUEST_HANDLER_H_ */

/*
 * marker_request.h
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 */

#ifndef MARKER_REQUEST_H_
#define MARKER_REQUEST_H_

#include <boost/smart_ptr.hpp>
#include "request.h"
#include "../Model/marker_information.h"

/**
 * \brief A Marker Request is issued by a robot which wants to change its marker information.
 *
 * Notes:
 * The request cannot be changed after creating.
 */
class MarkerRequest : public Request {
public:
	/**
	 * constructs a new MarkerRequest.
	 * The request cannot be changed after constructing.
	 */
	MarkerRequest(Robot& robot,
	              boost::shared_ptr<MarkerInformation> requested_marker_information) : Request(robot),
	                                                                                   requested_marker_information_(requested_marker_information) {}

	/**
	 * Returns a constant reference to the requested marker information
	 * \Return A constant reference to the requested marker information
	 */
	const MarkerInformation& requested_marker_information() const {return *requested_marker_information_;}

private:
	boost::shared_ptr<MarkerInformation> requested_marker_information_;

};

#endif /* MARKER_REQUEST_H_ */

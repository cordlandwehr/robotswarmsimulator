/*
 * marker_change_request_handler.cc
 *
 *  Created on: Feb 25, 2009
 *      Author: peter
 */

#include "marker_change_request_handler.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/marker_change_request.h"


void MarkerChangeRequestHandler::handle_request_reliable(boost::shared_ptr<WorldInformation> world_information,
                                                         boost::shared_ptr<const Request> request) {
	using boost::any;
	using boost::dynamic_pointer_cast;
	using boost::shared_ptr;

	shared_ptr<const MarkerChangeRequest> change_request = dynamic_pointer_cast<const MarkerChangeRequest>(request);
	if (!change_request)
		throw std::invalid_argument("Not a marker-change request.");

	// get request data
	const MarkerIdentifier& marker_to_change = change_request->marker_to_change();
	const std::string& var_name = change_request->var_name();
	shared_ptr<const any> var_value = change_request->var_value();

	// fulfill request
	WorldObject& marker = world_information->get_according_marker(marker_to_change);
	MarkerInformation marker_information = marker.marker_information();

	// add/remove data item (depending on validness of pointer var_value)
	if (var_value)
		marker_information.add_data(var_name, *var_value);
	else
		marker_information.remove_data(var_name);
}


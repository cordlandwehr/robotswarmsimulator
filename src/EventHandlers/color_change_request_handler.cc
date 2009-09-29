/*
 * color_change_request_handler.cc
 *
 *  Created on: 02.07.2009
 *      Author: kamil
 */


#include "../EventHandlers/color_change_request_handler.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Requests/color_change_request.h"
#include "../Model/robot_data.h"

bool ColorChangeRequestHandler::handle_request_reliable(
    boost::shared_ptr<WorldInformation> world_information,
    boost::shared_ptr<const Request> request) {
	using boost::any;
	using boost::dynamic_pointer_cast;
	using boost::shared_ptr;

	shared_ptr<const ColorChangeRequest> change_request =
	    dynamic_pointer_cast<const ColorChangeRequest>(request);
	if (!change_request)
		throw std::invalid_argument("Not a color-change request.");

	unsigned int new_color = change_request->requested_color();
	RobotData & robot_data =
	    world_information->get_according_robot_data(
	        change_request->robot().id());

	robot_data.set_color(new_color);

	return true;
}


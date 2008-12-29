/*
 * look_event.cc
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#include "look_event.h"

void LookEvent::add_to_robot_subset(boost::shared_ptr<Robot> new_robot) {
	robot_subset_.push_back(new_robot);
}

//TODO(craupach): maybe this should be inline.
const vector<boost::shared_ptr<Robot> >& LookEvent::get_robot_subset() const {
	return robot_subset_;
}
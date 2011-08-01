/*
 * edge_view.cc
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#include "edge_view.h"
#include "../Model/world_information.h"
#include "../Model/identifier.h"
#include "../Model/marker_identifier.h"
#include "../Model/robot_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/world_object.h"
#include "../Model/edge.h"


#include <boost/foreach.hpp>

EdgeView::EdgeView() {
}

EdgeView::~EdgeView() {
}

std::vector<View::RobotRef> EdgeView::get_visible_robots(const RobotData& robot) const {
	std::vector<View::RobotRef> result;
	const std::vector<boost::shared_ptr<EdgeIdentifier> > edges = robot.get_edges();
	const WorldInformation& wi = world_information();
	BOOST_FOREACH(const boost::shared_ptr<EdgeIdentifier>& ei, edges) {
		boost::shared_ptr<const Edge> e = wi.get_according_edge(ei);
		result.push_back(e->getRobot1());
		result.push_back(e->getRobot2());
	}
	//remove self
	result.erase(std::find(result.begin(), result.end(), boost::static_pointer_cast<RobotIdentifier>(robot.id())));
	return result;
}

std::vector<View::EdgeRef> EdgeView::get_visible_edges(const RobotData& robot) const {
	std::vector<View::EdgeRef> result = robot.get_edges();
	return result;
}

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
#include "../Model/directed_edge.h"
#include "../Model/robot.h"

#include <boost/foreach.hpp>

class Robot;

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
		// Check if edge is directed
		if(boost::shared_ptr<const DirectedEdge> d_edge = boost::dynamic_pointer_cast<const DirectedEdge> (e)) {
			if(d_edge->source()->id() == robot.robot().id()->id() && d_edge->target()->id() != robot.robot().id()->id()){
				result.push_back(d_edge->target());
			}
		} else {
			if(e->getRobot1()->id() != robot.robot().id()->id())
				result.push_back(e->getRobot1());
			else if(e->getRobot2()->id() != robot.robot().id()->id())
				result.push_back(e->getRobot2());
		}
	}
	return result;
}

std::vector<View::EdgeRef> EdgeView::get_visible_edges(const RobotData& robot) const {
	std::vector<View::EdgeRef> all_edges = robot.get_edges();
	std::vector<View::EdgeRef> result;
	BOOST_FOREACH(View::EdgeRef e_id, all_edges){
		boost::shared_ptr<const Edge> e = world_information().get_according_edge(e_id);
		// check if edge is directed or undirected
		if(boost::shared_ptr<const DirectedEdge> de = boost::dynamic_pointer_cast<const DirectedEdge>(e)){
			// check if robot is source of directed edge
			if(de->source()->id() == robot.id()->id())
				result.push_back(e_id);
		} else {
			result.push_back(e_id);
		}
	}
	return result;
}

bool EdgeView::is_edge_directed(const Edge& edge) const {
	if(typeid(edge) == typeid(DirectedEdge)){
		return true;
	} else {
		return false;
	}
}

View::RobotRef EdgeView::get_edge_source(const Edge& edge) const{
	if(typeid(edge) == typeid(DirectedEdge)) {
		DirectedEdge de = dynamic_cast<const DirectedEdge&>(edge);
		return de.source();
	} else {
		return edge.getRobot1();
	}
}

View::RobotRef EdgeView::get_edge_target(const Edge& edge) const{
	if(typeid(edge) == typeid(DirectedEdge)) {
		DirectedEdge de = dynamic_cast<const DirectedEdge&>(edge);
		return de.target();
	} else {
		return edge.getRobot2();
	}
}

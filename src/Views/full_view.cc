/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * full_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#include "full_view.h"
#include "../Model/world_information.h"
#include "../Model/identifier.h"
#include "../Model/marker_identifier.h"
#include "../Model/robot_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/world_object.h"
#include "../Model/obstacle.h"
#include "../Model/edge.h"
#include "../Model/directed_edge.h"


#include <boost/foreach.hpp>


namespace {
	template<typename S, typename T>
	std::vector<boost::shared_ptr<S> > convert_to_set(const std::vector<boost::shared_ptr<T> >& vec) {
		std::vector<boost::shared_ptr<S> > set;
		BOOST_FOREACH(const boost::shared_ptr<T>& t, vec) {
			set.push_back(boost::static_pointer_cast<S>(t->id()));
		}
		return set;
	}
}

FullView::FullView() {


}

FullView::~FullView() {

}

std::vector<View::RobotRef> FullView::get_visible_robots(const RobotData& robot) const {
	//put all RobotDatas into the set (if not the robot itself)
	std::vector<View::RobotRef> set;
	for (std::map< std::size_t, boost::shared_ptr < RobotData> >::const_iterator it = world_information().robot_data().begin(); it != world_information().robot_data().end(); ++it) {
	  if (!(it->second->id()->id()) == robot.id()->id())
		set.push_back(View::RobotRef(new RobotIdentifier(it->second->id()->id()))); //TODO asetzer I have no idea what I am doing here... :D
	}
  
	//std::vector<View::RobotRef> set = convert_to_set<RobotIdentifier, RobotData>(world_information().robot_data());
	//remove self
	//set.erase(std::find(set.begin(), set.end(), boost::static_pointer_cast<RobotIdentifier>(robot.id())));
	return set;
}
std::vector<View::ObstacleRef> FullView::get_visible_obstacles(const RobotData& robot) const {
	return convert_to_set<ObstacleIdentifier, Obstacle>(world_information().obstacles());
}
std::vector<View::MarkerRef> FullView::get_visible_markers(const RobotData& robot) const {
	return convert_to_set<MarkerIdentifier, WorldObject>(world_information().markers());
}

typedef std::map<std::size_t, boost::shared_ptr<Edge> > edge_map;

std::vector<View::EdgeRef> FullView::get_visible_edges(const RobotData& robot) const {
	edge_map all_edges = world_information().edges();
	std::vector<View::EdgeRef> result;
	BOOST_FOREACH(edge_map::value_type& e, all_edges){
		result.push_back(boost::dynamic_pointer_cast<EdgeIdentifier>(e.second->id()));
	}
	return result;
}

bool FullView::is_edge_directed(const Edge& edge) const {
	if(typeid(edge) == typeid(DirectedEdge)){
		return true;
	} else {
		return false;
	}
}

View::RobotRef FullView::get_edge_source(const Edge& edge) const{
	if(typeid(edge) == typeid(DirectedEdge)) {
		DirectedEdge de = dynamic_cast<const DirectedEdge&>(edge);
		return de.source();
	} else {
		return edge.robot1();
	}
}

View::RobotRef FullView::get_edge_target(const Edge& edge) const{
	if(typeid(edge) == typeid(DirectedEdge)) {
		DirectedEdge de = dynamic_cast<const DirectedEdge&>(edge);
		return de.target();
	} else {
		return edge.robot2();
	}
}

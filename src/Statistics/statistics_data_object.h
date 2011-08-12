/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 * statistics_data_object.h
 *
 *  Created on: Apr 27, 2009
 *      Author: craupach
 */

#ifndef STATISTICS_DATA_OBJECT_H_
#define STATISTICS_DATA_OBJECT_H_

#include <boost/graph/adjacency_list.hpp>
#include <iostream>

// class StatisticsDataObject {
// public:
// 	StatisticsDataObject() {}
// 	StatisticsDataObject(const StatisticsDataObject& rhs) : components_(new std::vector<int>(rhs.components())),
// 	                                                        vis_graph_(new boost::adjacency_list <> (rhs.vis_graph())),
// 	                                                        vis_graph_is_connected_(rhs.vis_graph_is_connected()) {}
// 
// 	void set_visibility_graph(boost::shared_ptr<boost::adjacency_list <> > vis_graph,
// 	                          size_t vis_graph_is_connected,
// 	                          boost::shared_ptr<std::vector<int> > components) {
// 		vis_graph_ = vis_graph;
// 		vis_graph_is_connected_ = vis_graph_is_connected;
// 		components_ = components;
// 	}
// 
// 	const boost::adjacency_list <> &  vis_graph() const {return *vis_graph_;}
// 	const boost::shared_ptr<boost::adjacency_list <> > vis_graph_ptr() const {return vis_graph_;}
// 	const size_t vis_graph_is_connected() const {return vis_graph_is_connected_;}
// 	const std::vector<int> & components() const {return *components_;}
// 
// private:
// 	/**
// 	 * Array indicating for each robot its connected component
// 	 */
// 	boost::shared_ptr<std::vector<int> > components_;
// 
// 	/**
// 	 * The visbility graph itsself
// 	 */
// 	boost::shared_ptr<boost::adjacency_list <> > vis_graph_;
// 
// 	/**
// 	 * This is true iff the graph is connected
// 	 */
// 	size_t vis_graph_is_connected_;
// };

#endif /* STATISTICS_DATA_OBJECT_H_ */

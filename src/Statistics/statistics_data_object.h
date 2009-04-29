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

/**
 * A StatisticsDataObject holds all data that should be passed from statistics to visualisation.
 * At the moment this is only the visibility graph.
 * This should be made more general in the future!
 */
class StatisticsDataObject {
public:
	StatisticsDataObject() {}
	StatisticsDataObject(const StatisticsDataObject& rhs) : components_(new std::vector<int>(rhs.components())),
	                                                        vis_graph_(new boost::adjacency_list <> (rhs.vis_graph())),
	                                                        vis_graph_is_connected_(rhs.vis_graph_is_connected()) {}

	void set_visibility_graph(boost::shared_ptr<boost::adjacency_list <> > vis_graph,
	                          size_t vis_graph_is_connected,
	                          boost::shared_ptr<std::vector<int> > components) {
		vis_graph_ = vis_graph;
		vis_graph_is_connected_ = vis_graph_is_connected;
		components_ = components;
	}

	const boost::adjacency_list <> &  vis_graph() const {return *vis_graph_;}
	const boost::shared_ptr<boost::adjacency_list <> > vis_graph_ptr() const {return vis_graph_;}
	const size_t vis_graph_is_connected() const {return vis_graph_is_connected_;}
	const std::vector<int> & components() const {return *components_;}

private:
	/**
	 * I don't know what this was for...
	 */
	boost::shared_ptr<std::vector<int> > components_;

	/**
	 * The visbility graph itsself
	 */
	boost::shared_ptr<boost::adjacency_list <> > vis_graph_;

	/**
	 * This is true iff the graph is connected
	 */
	size_t vis_graph_is_connected_;
};

#endif /* STATISTICS_DATA_OBJECT_H_ */

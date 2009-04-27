/*
 * statistics_data_object.h
 *
 *  Created on: Apr 27, 2009
 *      Author: craupach
 */

#ifndef STATISTICS_DATA_OBJECT_H_
#define STATISTICS_DATA_OBJECT_H_

#include <boost/graph/adjacency_list.hpp>

/**
 * A StatisticsDataObject holds all data that should be passed from statistics to visualisation.
 * At the moment this is only the visibility graph.
 * This should be made more general in the future!
 */
class StatisticsDataObject {
	void set_visibility_graph(boost::shared_ptr<boost::adjacency_list <> > vis_graph,
	                          size_t vis_graph_is_connected) {
		vis_graph_ = vis_graph;
		vis_graph_is_connected_;
	}

	const boost::adjacency_list <> & vis_graph() const {return *vis_graph_;}
	const size_t vis_graph_is_connected() const {return vis_graph_is_connected_;}

private:
	boost::shared_ptr<boost::adjacency_list <> > vis_graph_;
	size_t vis_graph_is_connected_;
};

#endif /* STATISTICS_DATA_OBJECT_H_ */

/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>

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

/**
 * \class	stats_calc
 * \author	Tobias Isenberg
 * \brief	performs all the statistical calculations
 *
 * The stats_calc-class implements the pure calculation of needed statistics.
 */


#ifndef STATS_CALC_H_
#define STATS_CALC_H_

#include <boost/smart_ptr.hpp>

#include "../Model/world_information.h"
#include "../Utilities/console_output.h"


class StatsCalc {
public:

	static const bool DEBUG = false;

	  /**
	   * Calculates the degree of a graph.
	   *
	   * \param graph	The graph as a WorldInformation
	   * \returns		degree
	   */
	static int calculate_degree(const boost::shared_ptr<WorldInformation> graph);

	  /**
	   * Calculates the maximal number of defects of a single node in the graph.
	   *
	   * \param graph	The graph as a WorldInformation
	   * \returns		maximal number of defects
	   */
	static int calculate_maximal_defect(const boost::shared_ptr<WorldInformation> graph);

	  /**
	   * Calculates the total number of defects in a graph.
	   *
	   * \param graph	The graph as a WorldInformation
	   * \returns		total number of defects
	   */
	static int calculate_total_defects(const boost::shared_ptr<WorldInformation> graph);

	  /**
	   * Calculates the hop distance between two nodes in a graph not using ignored edges.
	   *
	   * \param graph	The graph as a WorldInformation
	   * \param start 	The Identifier of the starting-node (source)
	   * \param target	The Identifier of the target-node	(sink)
	   * \param ignore	The Identifiers of the edges to be ignored
	   * \returns		hop distance
	   */
	static std::size_t calculate_hop_distance(const boost::shared_ptr<WorldInformation> graph,
				 	 	 	 	 	 	 	 boost::shared_ptr<RobotIdentifier> start,
				 	 	 	 	 	 	 	 boost::shared_ptr<RobotIdentifier> target,
				 	 	 	 	 	 	 	 const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore);

	/**
	 * Calculates the local greedy routing distance between start and end in the graph.
	 * Edges declared as long range links (long_range_link entry in MarkerInformation) will
	 * be used in one direction only.
	 *
	 * \param graph		The graph as a WorldInformation
	 * \param start 	The Identifier of the starting-node
	 * \param end		The Identifier of the end-node
	 * \param dist_func	A function that calculated distances between two robots in the graph
	 */
	static std::size_t calculate_lrl_local_greedy_routing_distance(const boost::shared_ptr<WorldInformation> graph,
	                                                               boost::shared_ptr<RobotIdentifier> start,
	                                                               boost::shared_ptr<RobotIdentifier> end,
	                                                               double (*dist_func)(const boost::shared_ptr<WorldInformation>,
	                                                            		   boost::shared_ptr<RobotIdentifier>,
	                                                            		   boost::shared_ptr<RobotIdentifier>));

	/**
	 * Calculates distribution of hop distances (using a local greedy strategy) for the
	 * the first Move and Forget experiment.
	 * 
	 * \param graph		Current WorldInformation.
	 * \returns		A vector of doubles. Values: (Min, q.125, q.25, q.375, Median,
	 *			q.625, q.750, q.875, Max, Mean, SD, Skewness, Exess).
	 */
	static const std::vector<double> evaluate_first_mf_experiment(const boost::shared_ptr<WorldInformation> graph);
	
	/**
	 * Calculates hop distance in a circle where robots are sorted by ID.
	 * This is an example function for a dist_func of calculate_lrl_local_greedy_routing_distance
	 *
	 * \param graph		The graph as a WorldInformation
	 * \param start 	The Identifier of the starting-node
	 * \param end		The Identifier of the end-node
	 */
	static double normal_circle_dist_func(const boost::shared_ptr<WorldInformation> graph,
	                                      boost::shared_ptr<RobotIdentifier> start,
	                                      boost::shared_ptr<RobotIdentifier> end);
	
	static std::size_t calculate_diameter_and_ignore_long_range_links_in_one_dir(const boost::shared_ptr<WorldInformation> graph,
						 const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore);
	
	static std::size_t calculate_hop_distance_and_ignore_long_range_links_in_one_dir(const boost::shared_ptr<WorldInformation> graph,
											boost::shared_ptr<RobotIdentifier> start,
											boost::shared_ptr<RobotIdentifier> target,
											const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore);
	
private:
	static bool is_edge_in_list(std::vector< boost::shared_ptr<EdgeIdentifier> > ignore,
						boost::shared_ptr<EdgeIdentifier> find_this_edge);

	static std::vector<boost::shared_ptr<EdgeIdentifier> > outgoing_edges(const boost::shared_ptr<WorldInformation> wi,
																		boost::shared_ptr<RobotData> rd);

	static boost::shared_ptr<EdgeIdentifier> get_outgoing_lrl(const boost::shared_ptr<WorldInformation> wi,
															  boost::shared_ptr<RobotData> rd);
};

#endif /* STATS_CALC_H_ */

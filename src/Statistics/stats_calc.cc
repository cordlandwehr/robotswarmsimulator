/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
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

/*
 * stats_calc.cpp
 *
 *  Created on: 27.07.2011
 *      Author: Tobias Isenberg
 */


#include "stats_calc.h"

#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Model/edge_identifier.h"

#include <queue>

const std::string kColorName = ":color";


int StatsCalc::calculate_degree(const boost::shared_ptr<WorldInformation> graph){
	std::vector<boost::shared_ptr<RobotData> > nodes;
	graph->robot_data_to_vector(nodes);

	int degree = 0;
	for(int i=0; i<nodes.size();i++){
		boost::shared_ptr<RobotData> currentNode = nodes[i];
		int degreeOfCurrentNode = outgoing_edges(graph, currentNode).size();
		if(degree < degreeOfCurrentNode){
			degree = degreeOfCurrentNode;
		}
	}
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "stats_calc::calculated degree " << degree << ".";
	return degree;
}



int StatsCalc::calculate_maximal_defect(const boost::shared_ptr<WorldInformation> graph){
	std::vector<boost::shared_ptr<RobotData> > nodes;
	graph->robot_data_to_vector(nodes);


	int maxDefect = 0;
		for(int i=0; i<nodes.size();i++){
			boost::shared_ptr<RobotData> currentNode = nodes[i];
			int degreeOfCurrentNode = (currentNode->get_edges()).size();
			std::vector<boost::shared_ptr<EdgeIdentifier> > edges = currentNode->get_edges();

			MarkerInformation currNodeMarker = currentNode->marker_information();
			if(!currNodeMarker.has_key(kColorName))
				return -1;
			double currentNodesOwnColor = boost::any_cast<double>(currNodeMarker.get_data(kColorName));

			int countDefects = 0;
			for(int j=0;j<degreeOfCurrentNode;j++){
				boost::shared_ptr<Edge> e = graph->get_according_edge(edges[j]);

				RobotData& rd1 = graph->get_according_robot_data(e->robot1());
				RobotData& rd2 = graph->get_according_robot_data(e->robot2());

				currNodeMarker = rd1.marker_information();
				if(!currNodeMarker.has_key(kColorName))
								return -1;
				double neighboursColor = boost::any_cast<double>(currNodeMarker.get_data(kColorName));

				if(neighboursColor == currentNodesOwnColor)
					countDefects ++;

				currNodeMarker = rd2.marker_information();
				if(!currNodeMarker.has_key(kColorName))
								return -1;
				neighboursColor = boost::any_cast<double>(currNodeMarker.get_data(kColorName));
				if(neighboursColor == currentNodesOwnColor)
					countDefects ++;

				//one neighbour is the node itself -> decrease count by 1
				countDefects --;
			}
			if(maxDefect < countDefects){
				maxDefect = countDefects;
			}
		}
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "stats_calc::calculated maximal defect " << maxDefect << ".";
		return maxDefect;
}

int StatsCalc::calculate_total_defects(const boost::shared_ptr<WorldInformation> graph){
	const std::map<std::size_t, boost::shared_ptr<Edge> > edges = graph->edges();

	std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator it = edges.begin();

	int countDefects = 0;
	while(it != edges.end()){
		boost::shared_ptr<Edge> currentEdge = it->second;

		RobotData& rd1 = graph->get_according_robot_data(currentEdge->robot1());
		RobotData& rd2 = graph->get_according_robot_data(currentEdge->robot2());

		MarkerInformation currNodeMarker = rd1.marker_information();
		if(!currNodeMarker.has_key(kColorName))
						return -1;
		double neighboursColor1 = boost::any_cast<double>(currNodeMarker.get_data(kColorName));
		currNodeMarker = rd2.marker_information();
		if(!currNodeMarker.has_key(kColorName))
						return -1;
		double neighboursColor2 = boost::any_cast<double>(currNodeMarker.get_data(kColorName));
		if(neighboursColor1 == neighboursColor2)
			countDefects ++;

		it++;
	}

	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "stats_calc::calculated total defect " << countDefects << ".";
	return countDefects;
}

std::size_t StatsCalc::calculate_hop_distance(const boost::shared_ptr<WorldInformation> graph,
						 boost::shared_ptr<RobotIdentifier> start,
						 boost::shared_ptr<RobotIdentifier> target,
						 const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore){

	if(start->id() == target->id()){
		return 0;
	}

	boost::shared_ptr<RobotData> start_node = graph->get_according_robot_data_ptr(start);
	boost::shared_ptr<RobotData> target_node = graph->get_according_robot_data_ptr(target);


	//no nodes are visited at the start
	std::map<size_t, bool> node_visited;
	std::map<size_t, int> hops_to_node;


	for (std::map< std::size_t, boost::shared_ptr < RobotData> >::const_iterator it =  graph->robot_data().begin(); it !=  graph->robot_data().end(); ++it) {
	  node_visited[it->first] = false;
	}
	
	/*std::vector<boost::shared_ptr<RobotData> > all_robots = graph->robot_data();
	for(int i=0; i<all_robots.size();i++){
		node_visited[all_robots[i]]=false;
	}*/

	//initialize queue
	std::queue<boost::shared_ptr<RobotData> > node_queue;
	boost::shared_ptr<RobotData> current_node = start_node;

	//start with start node
	node_queue.push(current_node);
	node_visited[current_node->id()->id()] = true;
	hops_to_node[current_node->id()->id()] = 0;

	while(! node_queue.empty()){
		current_node = node_queue.front();
		node_queue.pop();

		std::vector<boost::shared_ptr<EdgeIdentifier> > edges_of_current_nodes = outgoing_edges(graph, current_node);
		int degree_of_current_node = edges_of_current_nodes.size();

		for(int j=0;j<degree_of_current_node;j++){
			boost::shared_ptr<EdgeIdentifier> current_edge_ID = edges_of_current_nodes[j];
			bool found = is_edge_in_list(ignore, current_edge_ID);
			if(!found){
				boost::shared_ptr<Edge> e = graph->get_according_edge(current_edge_ID);

				boost::shared_ptr<RobotData> rd1 = graph->get_according_robot_data_ptr(e->robot1());
				boost::shared_ptr<RobotData> rd2 = graph->get_according_robot_data_ptr(e->robot2());

				boost::shared_ptr<RobotData> child;
				if(rd1 == current_node)
					child = rd2;
				else
					child = rd1;

				if(!node_visited[child->id()->id()]){
					if(child == target_node){
						ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "stats_calc::calculated hop distance " << hops_to_node[current_node->id()->id()]+1 << ".";
						return hops_to_node[current_node->id()->id()]+1;
					}
					node_queue.push(child);
					node_visited[child->id()->id()] = true;
					hops_to_node[child->id()->id()] = hops_to_node[current_node->id()->id()]+1;
				}
			}
		}

	}
	return -1;
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "stats_calc::calculated hop distance -- target not reachable.";
}

std::size_t StatsCalc::calculate_lrl_local_greedy_routing_distance(const boost::shared_ptr<WorldInformation> graph,
	                                                               boost::shared_ptr<RobotIdentifier> start,
	                                                               boost::shared_ptr<RobotIdentifier> end,
	                                                               double (*dist_func)(const boost::shared_ptr<WorldInformation>,
	                                                            		   	   	   	   boost::shared_ptr<RobotIdentifier>,
	                                                            		   	   	   	   boost::shared_ptr<RobotIdentifier>)){
	boost::shared_ptr<RobotIdentifier> current_node = start;
	std::size_t hops = 0;

	while(current_node->id() != end->id()){
		boost::shared_ptr<RobotData> current_node_rd = graph->get_according_robot_data_ptr(current_node);

		// get list of all possible next robots
		std::vector<boost::shared_ptr<RobotIdentifier> > next_hops;
		std::vector<boost::shared_ptr<EdgeIdentifier> > edges = outgoing_edges(graph, current_node_rd);
		for(int i=0; i<edges.size(); ++i){
			boost::shared_ptr<Edge> e = graph->get_according_edge(edges[i]);
			if((!e->marker_information().has_key("long_range_link")) || (e->marker_information().has_key("long_range_link") && e->robot1()->id() == current_node->id())){
				if(e->robot1()->id() != current_node->id()){
					next_hops.push_back(e->robot1());
				} else if(e->robot2()->id() != current_node->id()){
					next_hops.push_back(e->robot2());
				}
			}
		}

		// calculate distances of possible next hops to end node
		std::vector<double> dist;
		for(int i=0; i<next_hops.size(); ++i){
			dist.push_back(dist_func(graph, next_hops[i], end));
		}

		// find next hop closest to end node
		double min = dist[0];
		int min_index = 0;
		for(int i=1; i<dist.size(); ++i){
			if(dist[i] < min){
				min = dist[i];
				min_index = i;
			}
		}

		// iterate to next node
		current_node = next_hops[min_index];
		hops++;
	}

	return hops;
}

const std::vector<double> StatsCalc::evaluate_first_mf_experiment(const boost::shared_ptr<WorldInformation> graph) {
  // vector to store all hopdistaces in
  std::vector<std::size_t> hop_distances;
  // iterate over all possible pairs
  std::map< std::size_t, boost::shared_ptr<RobotData> >::iterator ita, itb;
  for (ita = graph->robot_data().begin();
       ita != graph->robot_data().end();
       ++ita) {
    for (itb = graph->robot_data().begin();
	 itb != graph->robot_data().end();
	 ++itb) {
      // skip cases where source and target are the same robot
      if (ita->first == itb->first) continue;
      // calculate lrl greedy local hop distance
      std::size_t distance = calculate_lrl_local_greedy_routing_distance(graph,
									 boost::dynamic_pointer_cast<RobotIdentifier>(ita->second->id()),
									 boost::dynamic_pointer_cast<RobotIdentifier>(itb->second->id()),
									 &StatsCalc::normal_circle_dist_func);
      hop_distances.push_back(distance);
    }
  }
  // sort the vector (for median calculations)
  std::sort(hop_distances.begin(), hop_distances.end());
  // calculate min, max , quantiles and median...
  std::size_t n = hop_distances.size();
  double min = hop_distances.at(0);
  double max = hop_distances.at(n-1);
  double q125, q250, q375, median, q625, q750, q875;
  // TODO: Do this in a nice loop?!
  if (n % 2 == 0) {
    median = (double)(hop_distances.at((std::size_t)(n*.5)-1)+hop_distances.at((std::size_t)(n*.5)))/2;
  } else {
    median = hop_distances.at(n/2);
  }
  if (n % 4 == 0) {
    q250 = (double)(hop_distances.at((std::size_t)(n*.25)-1)+hop_distances.at((std::size_t)(n*.25)))/2;
    q750 = (double)(hop_distances.at((std::size_t)(n*.75)-1)+hop_distances.at((std::size_t)(n*.75)))/2;
  } else {
    q250 = hop_distances.at((std::size_t)(n*.25));
    q750 = hop_distances.at((std::size_t)(n*.75));
  }
  if (n % 8 == 0) {
    q125 = (double)(hop_distances.at((std::size_t)(n*.125)-1)+hop_distances.at((std::size_t)(n*.125)))/2;
    q375 = (double)(hop_distances.at((std::size_t)(n*.375)-1)+hop_distances.at((std::size_t)(n*.375)))/2;
    q625 = (double)(hop_distances.at((std::size_t)(n*.625)-1)+hop_distances.at((std::size_t)(n*.625)))/2;
    q875 = (double)(hop_distances.at((std::size_t)(n*.875)-1)+hop_distances.at((std::size_t)(n*.875)))/2;
  } else {
    q125 = hop_distances.at((std::size_t)(n*.125));
    q375 = hop_distances.at((std::size_t)(n*.375));
    q625 = hop_distances.at((std::size_t)(n*.625));
    q875 = hop_distances.at((std::size_t)(n*.875));
  }
  // calculate mean
  double mean = 0.0;
  std::vector<std::size_t>::const_iterator it;
  for (it = hop_distances.begin();
       it != hop_distances.end();
       ++it) {
    mean += *it;
  }
  mean /= n;
  // calc distribution
  std::size_t nn = graph->robot_data().size()/2;
  std::vector<std::size_t> counts(nn, 0);
  // calculate SD etc.
  double sum_of_square_diffs = 0.0;
  double sum_of_cubic_diffs = 0.0;
  double sum_of_4th_diffs = 0.0;
  for (it = hop_distances.begin();
       it != hop_distances.end();
       ++it) {
    // due to performance reason I do not use std::pow in this loop
    // I hope the compiler will optimze this for me ;-)
    sum_of_square_diffs += (*it-mean)*(*it-mean);
    sum_of_cubic_diffs += (*it-mean)*(*it-mean)*(*it-mean);
    sum_of_4th_diffs += (*it-mean)*(*it-mean)*(*it-mean)*(*it-mean);
    // count hop distance
    counts[*it-1] += 1;
  }
  double sd = std::sqrt(sum_of_square_diffs/(n-1));
  double skewness = (sum_of_cubic_diffs/n) / std::sqrt(std::pow(sum_of_square_diffs/n, 3.0));
  double excess = ((sum_of_4th_diffs/n) / std::pow(sum_of_square_diffs/n, 2.0))-3.0;
  
  // result vetor
  std::vector<double> result;
  result.push_back(min);
  result.push_back(q125);
  result.push_back(q250);
  result.push_back(q375);
  result.push_back(median);
  result.push_back(q625);
  result.push_back(q750);
  result.push_back(q875);
  result.push_back(max);
  result.push_back(mean);
  result.push_back(sd);
  result.push_back(skewness);
  result.push_back(excess);
  result.insert(result.end(), counts.begin(), counts.end());
  // return results
  return result;
}


const std::vector<double> StatsCalc::evaluate_uniform_lrlink_distribution(const boost::shared_ptr<WorldInformation> graph) {
  // vector to store all hopdistaces in
  std::vector< std::vector<double> > hop_distances(graph->robot_data().size()/2);
  // iterate over all possible pairs
  std::map< std::size_t, boost::shared_ptr<RobotData> >::iterator ita, itb;
  for (ita = graph->robot_data().begin();
       ita != graph->robot_data().end();
       ++ita) {
    for (itb = graph->robot_data().begin();
	 itb != graph->robot_data().end();
	 ++itb) {
      // skip cases where source and target are the same robot
      if (ita->first == itb->first) continue;
      // calculate lrl greedy local hop distance
      double distance = normal_circle_dist_func(graph,
						boost::dynamic_pointer_cast<RobotIdentifier>(ita->second->id()),
						boost::dynamic_pointer_cast<RobotIdentifier>(itb->second->id()));
      // skip distances smaller than two
      if (distance < 2.0) continue;
      // get long range link
      std::vector<boost::shared_ptr<EdgeIdentifier> > edges = outgoing_edges(graph, ita->second);
      boost::shared_ptr<RobotIdentifier> lr_partner;
      for(int i=0; i<edges.size(); ++i){
	boost::shared_ptr<Edge> e = graph->get_according_edge(edges[i]);
	if((!e->marker_information().has_key("long_range_link")) || (e->marker_information().has_key("long_range_link") && e->robot1()->id() == ita->second->id()->id())){
	  if(e->robot1()->id() != ita->second->id()->id()){
	    lr_partner = e->robot1();
	  } else if(e->robot2()->id() != ita->second->id()->id()){
	    lr_partner = e->robot2();
	  }
	}
      }
      
      // no lr link? reduction of 1
      if (!lr_partner) {
	hop_distances[(std::size_t)distance-1].push_back(1);
      } else {
	double lr_distance = normal_circle_dist_func(graph,
						    boost::dynamic_pointer_cast<RobotIdentifier>(ita->second->id()),
						    lr_partner);
	if (lr_distance < distance) {
	  hop_distances[(std::size_t)distance-1].push_back(distance-lr_distance);
	} else {
	  hop_distances[(std::size_t)distance-1].push_back(1);
	}
      }
    }
  }
  
  // compute mean and for each distance
  std::vector< std::pair<double, double> > avg_distances(hop_distances.size());
  
  // calculate mean
  std::vector< std::vector<double> >::const_iterator itd;
  std::vector<double>::const_iterator itv;
  
  for (std::size_t i = 0; i < hop_distances.size(); i++) {
    double mean = 0.0;
    for (std::size_t j = 0; j < hop_distances[i].size(); j++) {
      mean += hop_distances[i][j];
    }
    mean /= hop_distances[i].size();
    avg_distances[i].first = mean;
  }
  
  // calculate SD
  for (std::size_t i = 0; i < hop_distances.size(); i++) {
    double sd = 0.0;
    for (std::size_t j = 0; j < hop_distances[i].size(); j++) {
      double diff = hop_distances[i][j]-avg_distances[i].first;
      sd += diff*diff;
    }
    sd /= (hop_distances[i].size()-1);
    avg_distances[i].second = sd;
  }  
  
  // return results
  std::vector<double> result(2*avg_distances.size());
  for (std::size_t i = 0; i < avg_distances.size(); i++) {
    result[2*i] = avg_distances[i].first;
    result[2*i+1] = avg_distances[i].second;
  }
  
  return result;
}

double StatsCalc::normal_circle_dist_func(const boost::shared_ptr<WorldInformation> graph,
	                                      boost::shared_ptr<RobotIdentifier> start,
	                                      boost::shared_ptr<RobotIdentifier> end){
	std::size_t circle_size = graph->robot_data().size();

	double result;
	if(start->id() > end->id()){
		result = start->id() - end->id();
	} else {
		result = end->id() - start->id();
	}

	if(result > circle_size/2.0){
		result = circle_size - result;
	}

	return result;
}

bool StatsCalc::is_edge_in_list(std::vector< boost::shared_ptr<EdgeIdentifier> > ignore,
					boost::shared_ptr<EdgeIdentifier> find_this_edge){

	for(int i=0; i<ignore.size();i++){
		boost::shared_ptr<EdgeIdentifier> current_ignore_edge = ignore[i];
		if(current_ignore_edge->id() == find_this_edge->id())
			return true;
	}
	return false;

}

std::vector<boost::shared_ptr<EdgeIdentifier> > StatsCalc::outgoing_edges(const boost::shared_ptr<WorldInformation> wi,
															   boost::shared_ptr<RobotData> rd) {
	std::vector<boost::shared_ptr<EdgeIdentifier> > edges = rd->get_edges();

	std::vector<boost::shared_ptr<EdgeIdentifier> > outgoing_edges;

	for(int i=0; i<edges.size(); i++){
		boost::shared_ptr<Edge> e = wi->get_according_edge(edges[i]);
		if(boost::shared_ptr<DirectedEdge> de = boost::dynamic_pointer_cast<DirectedEdge>(e)) {
			if(de->source()->id() == rd->id()->id())
				outgoing_edges.push_back(edges[i]);
		} else {
			outgoing_edges.push_back(edges[i]);
		}
	}

	return outgoing_edges;
}


std::size_t StatsCalc::calculate_diameter_and_ignore_long_range_links_in_one_dir(const boost::shared_ptr<WorldInformation> graph,
						 const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore) {
  size_t max_distance = 0;
  for (std::map< std::size_t, boost::shared_ptr < RobotData> >::const_iterator it1 =  graph->robot_data().begin(); it1 !=  graph->robot_data().end(); ++it1) {
	for (std::map< std::size_t, boost::shared_ptr < RobotData> >::const_iterator it2 =  graph->robot_data().begin(); it2 !=  graph->robot_data().end(); ++it2) {
	  if (it1 != it2) {
			boost::shared_ptr<RobotData> rd1 = it1->second;
			boost::shared_ptr<RobotData> rd2 = it2->second;
			
			boost::shared_ptr<RobotIdentifier> ri1 (new RobotIdentifier(rd1->id()->id()));
			boost::shared_ptr<RobotIdentifier> ri2 (new RobotIdentifier(rd2->id()->id()));

			size_t new_distance = calculate_hop_distance_and_ignore_long_range_links_in_one_dir(graph, ri1, ri2, ignore );
			if (new_distance > max_distance)
			  max_distance = new_distance;
	  }
	}
  }
  return max_distance;
}

std::size_t StatsCalc::calculate_hop_distance_and_ignore_long_range_links_in_one_dir(const boost::shared_ptr<WorldInformation> graph,
						 boost::shared_ptr<RobotIdentifier> start,
						 boost::shared_ptr<RobotIdentifier> target,
						 const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore){

	if(start->id() == target->id()){
		return 0;
	}

	boost::shared_ptr<RobotData> start_node = graph->get_according_robot_data_ptr(start);
	boost::shared_ptr<RobotData> target_node = graph->get_according_robot_data_ptr(target);


	//no nodes are visited at the start
	std::map<size_t, bool> node_visited;
	std::map<size_t, int> hops_to_node;


	for (std::map< std::size_t, boost::shared_ptr < RobotData> >::const_iterator it =  graph->robot_data().begin(); it !=  graph->robot_data().end(); ++it) {
	  node_visited[it->first] = false;
	}
	
	/*std::vector<boost::shared_ptr<RobotData> > all_robots = graph->robot_data();
	for(int i=0; i<all_robots.size();i++){
		node_visited[all_robots[i]]=false;
	}*/

	//initialize queue
	std::queue<boost::shared_ptr<RobotData> > node_queue;
	boost::shared_ptr<RobotData> current_node = start_node;

	//start with start node
	node_queue.push(current_node);
	node_visited[current_node->id()->id()] = true;
	hops_to_node[current_node->id()->id()] = 0;

	while(! node_queue.empty()){
		current_node = node_queue.front();
		node_queue.pop();

		std::vector<boost::shared_ptr<EdgeIdentifier> > edges_of_current_nodes = outgoing_edges(graph, current_node);
		int degree_of_current_node = edges_of_current_nodes.size();

		for(int j=0;j<degree_of_current_node;j++){
			boost::shared_ptr<EdgeIdentifier> current_edge_ID = edges_of_current_nodes[j];		
			bool found = is_edge_in_list(ignore, current_edge_ID);
			if(!found){
				boost::shared_ptr<Edge> e = graph->get_according_edge(current_edge_ID);

				//if the edge is a longe range link, but in the wrong direction, don't go this path
				if (e->marker_information().has_key("long_range_link")) {
				  if (e->robot1()->id() != current_node->id()->id())
					continue;
				}

				boost::shared_ptr<RobotData> rd1 = graph->get_according_robot_data_ptr(e->robot1());
				boost::shared_ptr<RobotData> rd2 = graph->get_according_robot_data_ptr(e->robot2());

				boost::shared_ptr<RobotData> child;
				if(rd1 == current_node)
					child = rd2;
				else
					child = rd1;

				if(!node_visited[child->id()->id()]){
					if(child == target_node){
						ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "stats_calc::calculated hop distance " << hops_to_node[current_node->id()->id()]+1 << ".";
						return hops_to_node[current_node->id()->id()]+1;
					}
					node_queue.push(child);
					node_visited[child->id()->id()] = true;
					hops_to_node[child->id()->id()] = hops_to_node[current_node->id()->id()]+1;
				}
			}
		}

	}
	return -1;
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) << "stats_calc::calculated hop distance -- target not reachable.";
}



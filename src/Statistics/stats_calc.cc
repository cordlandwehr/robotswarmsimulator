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

#include <queue>


int StatsCalc::calculate_degree(const boost::shared_ptr<WorldInformation> graph){
	const std::vector<boost::shared_ptr<RobotData> >& nodes = graph->robot_data();

	int degree = 0;
	for(int i=0; i<nodes.size();i++){
		boost::shared_ptr<RobotData> currentNode = nodes[i];
		int degreeOfCurrentNode = (currentNode->get_edges()).size();
		if(degree < degreeOfCurrentNode){
			degree = degreeOfCurrentNode;
		}
	}
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "stats_calc::calculated degree " << degree << ".";
	return degree;
}



int StatsCalc::calculate_maximal_defect(const boost::shared_ptr<WorldInformation> graph){
	const std::vector<boost::shared_ptr<RobotData> >& nodes = graph->robot_data();

	int maxDefect = 0;
		for(int i=0; i<nodes.size();i++){
			boost::shared_ptr<RobotData> currentNode = nodes[i];
			int degreeOfCurrentNode = (currentNode->get_edges()).size();
			std::vector<boost::shared_ptr<EdgeIdentifier> > edges = currentNode->get_edges();
			unsigned short int currentNodesOwnColor = currentNode->color();
			int countDefects = 0;
			for(int j=0;j<degreeOfCurrentNode;j++){
				boost::shared_ptr<Edge> e = graph->get_according_edge(edges[j]);

				RobotData& rd1 = graph->get_according_robot_data(e->getRobot1());
				RobotData& rd2 = graph->get_according_robot_data(e->getRobot2());

				unsigned short int neighboursColor = rd1.color();
				if(neighboursColor == currentNodesOwnColor)
					countDefects ++;

				neighboursColor = rd2.color();
				if(neighboursColor == currentNodesOwnColor)
					countDefects ++;

				//one neighbour is the node itself -> decrease count by 1
				countDefects --;
			}
			if(maxDefect < countDefects){
				maxDefect = countDefects;
			}
		}
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "stats_calc::calculated maximal defect " << maxDefect << ".";
		return maxDefect;
}

int StatsCalc::calculate_total_defects(const boost::shared_ptr<WorldInformation> graph){
	const std::map<std::size_t, boost::shared_ptr<Edge> > edges = graph->edges();

	std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator it = edges.begin();

	int countDefects = 0;
	while(it != edges.end()){
		boost::shared_ptr<Edge> currentEdge = it->second;

		RobotData& rd1 = graph->get_according_robot_data(currentEdge->getRobot1());
		RobotData& rd2 = graph->get_according_robot_data(currentEdge->getRobot2());

		unsigned short int neighboursColor1 = rd1.color();
		unsigned short int neighboursColor2 = rd2.color();
		if(neighboursColor1 == neighboursColor2)
			countDefects ++;

		it++;
	}

	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "stats_calc::calculated total defect " << countDefects << ".";
	return countDefects;
}

std::size_t StatsCalc::calculate_hop_distance(const boost::shared_ptr<WorldInformation> graph,
						 boost::shared_ptr<RobotIdentifier> start,
						 boost::shared_ptr<RobotIdentifier> target,
						 const std::vector< boost::shared_ptr<EdgeIdentifier> >& ignore){

	if(start == target){
		return 0;
	}

	boost::shared_ptr<RobotData> start_node = graph->get_according_robot_data_ptr(start);
	boost::shared_ptr<RobotData> target_node = graph->get_according_robot_data_ptr(target);


	//no nodes are visited at the start
	std::map<boost::shared_ptr<RobotData>, bool> node_visited;
	std::map<boost::shared_ptr<RobotData>, int> hops_to_node;

	std::vector<boost::shared_ptr<RobotData> > all_robots = graph->robot_data();
	for(int i=0; i<all_robots.size();i++){
		node_visited[all_robots[i]]=false;
	}

	//initialize queue
	std::queue<boost::shared_ptr<RobotData> > node_queue;
	boost::shared_ptr<RobotData> current_node = start_node;

	//start with start node
	node_queue.push(current_node);
	node_visited[current_node] = true;
	hops_to_node[current_node] = 0;

	while(! node_queue.empty()){
		current_node = node_queue.front();
		node_queue.pop();

		std::vector<boost::shared_ptr<EdgeIdentifier> > edges_of_current_nodes = current_node->get_edges();
		std::vector<boost::shared_ptr<EdgeIdentifier> >::const_iterator finding_iterator;
		int degree_of_current_node = edges_of_current_nodes.size();

		for(int j=0;j<degree_of_current_node;j++){
			boost::shared_ptr<EdgeIdentifier> current_edge_ID = edges_of_current_nodes[j];
			finding_iterator = std::find(ignore.begin(),ignore.end(),current_edge_ID);
			if(finding_iterator == ignore.end()){
				boost::shared_ptr<Edge> e = graph->get_according_edge(current_edge_ID);

				boost::shared_ptr<RobotData> rd1 = graph->get_according_robot_data_ptr(e->getRobot1());
				boost::shared_ptr<RobotData> rd2 = graph->get_according_robot_data_ptr(e->getRobot2());

				boost::shared_ptr<RobotData> child;
				if(rd1 == current_node)
					child = rd2;
				else
					child = rd1;

				if(!node_visited[child]){
					if(child == target_node){
						ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "stats_calc::calculated hop distance " << hops_to_node[current_node]+1 << ".";
						return hops_to_node[current_node]+1;
					}
					node_queue.push(child);
					node_visited[child] = true;
					hops_to_node[child] = hops_to_node[current_node]+1;
				}
			}
		}

	}
	return -1;
	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "stats_calc::calculated hop distance -- target not reachable.";
}

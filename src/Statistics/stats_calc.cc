/*
 * stats_calc.cpp
 *
 *  Created on: 27.07.2011
 *      Author: Tobias Isenberg
 */

#include <cmath>

#include "stats_calc.h"
#include "float.h"

#include "../Model/robot_data.h"
//#include "../Model/robot_identifier.h"
//#include "../Utilities/vector_arithmetics.h"
//#include "../Views/view.h"

StatsCalc::StatsCalc() {

}

StatsCalc::~StatsCalc() {

}

void StatsCalc::init(StatsConfig* stats_cfg) {
	this->stats_cfg_ = stats_cfg;
}

void StatsCalc::calculate(StatsCalcInData & data,
		boost::shared_ptr<StatsOut> & stats_out) {

	if (DEBUG)
		std::cout << "DEBUG: >>>> stats_calc::calculate(...)" << std::endl;

	const boost::shared_ptr<WorldInformation> graph = data.world_info_;
	const std::vector<boost::shared_ptr<RobotData> >& nodes = data.world_info_->robot_data();

	calculateDegree(nodes);
}


int StatsCalc::calculateDegree(const std::vector<boost::shared_ptr<RobotData> >& nodes){
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



int StatsCalc::calculateMaximalDefect(const boost::shared_ptr<WorldInformation> graph){
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

int StatsCalc::calculateTotalDefects(const boost::shared_ptr<WorldInformation> graph){
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

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

void StatsCalc::calculate(const StatsCalcInData & data,
		const boost::shared_ptr<StatsOut> & stats_out) {

	ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::debug) <<"stats_calc::calculate(...)";


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

#include <iostream>
#include <iomanip>
#include <ctime>
#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>


#include "../../Statistics/stats_calc.h"

#include "../../ViewModels/local_graph_view.h"

#include "../../Views/view_factory.h"
#include "../../Views/abstract_view_factory.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot_identifier.h"
#include "../../Model/robot.h"
#include "../../Model/undirected_edge.h"

#include "../../SimulationControl/history.h"
#include "../../SimulationControl/time_point.h"

#include "../Utilities/distribution_generator.h"

#include "../../RobotImplementations/simple_robot.h"

BOOST_AUTO_TEST_CASE(stats_calc_test) {

	boost::shared_ptr<WorldInformation> graph (new WorldInformation());
	boost::shared_ptr<History> history (new History(5));

	std::vector<boost::shared_ptr<RobotIdentifier> > IDs;

	boost::shared_ptr<Vector3d> pos(new Vector3d());
	pos->insert_element(kXCoord,0.0);
	pos->insert_element(kYCoord,0.0);
	pos->insert_element(kZCoord,0.0);

	//create 1000 Nodes and NodeDatas
	for(int i = 0; i < 1000; i++) {
		boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
		IDs.push_back(id);

		//set the color
		boost::shared_ptr<MarkerInformation> marker_information(new MarkerInformation());
		double color = static_cast<double>(i);;
		marker_information->add_data(":color",color);

		boost::shared_ptr<Robot> node(new SimpleRobot(id));
		boost::shared_ptr<RobotData> nodeData(new RobotData(id, pos, marker_information, node));

		graph->add_robot_data(nodeData);
	}

	BOOST_CHECK_EQUAL(graph->robot_data().size(), 1000);

	//create edge between consecutive nodes (gives a line)
	std::vector<boost::shared_ptr<RobotData> > nodes;
	graph->robot_data_to_vector(nodes);
	
	
	
	for(int i=1; i<nodes.size();i++){
		boost::shared_ptr<RobotIdentifier> nodeID1 = IDs[i-1];
		boost::shared_ptr<RobotIdentifier> nodeID2 = IDs[i];

		//create edge
		boost::shared_ptr<Edge> e (new UndirectedEdge(nodeID1,nodeID2));
		graph->add_edge(e);
	}
	// set time of inital world information
	graph->set_time(0);

	// insert initial world information into history
	boost::shared_ptr<TimePoint> new_time_point(new TimePoint());
	new_time_point->set_world_information(graph);
	history->insert(new_time_point);

	// setup of view and event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<LocalGraphView>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, graph));

	//setup views of robots
	for(int i=0; i<nodes.size();i++){
		RobotData& tempRobotData = graph->get_according_robot_data(IDs[i]);
		robot_control->compute_view(tempRobotData.robot());
	}

	boost::shared_ptr<DistributionGenerator> generator(new DistributionGenerator(0));
	View::set_distribution_generator(generator);

	//get StatsCalc class
	StatsCalc stats_calc_;

	/**************************************************
	 * the graph is a straight line of nodes 0 - 999  *
	 * so degree is two								  *
	 * and there are no defects						  *
	 **************************************************/
	BOOST_CHECK_EQUAL(stats_calc_.calculate_degree(history->get_newest().world_information_ptr()), 2);
	BOOST_CHECK_EQUAL(stats_calc_.calculate_maximal_defect(history->get_newest().world_information_ptr()),0);
	BOOST_CHECK_EQUAL(stats_calc_.calculate_total_defects(history->get_newest().world_information_ptr()),0);

	//add one directed edge
	boost::shared_ptr<RobotIdentifier> nodeID1 = IDs[1];
	boost::shared_ptr<RobotIdentifier> nodeID4 = IDs[4];
	//create edge
	boost::shared_ptr<DirectedEdge> e (new DirectedEdge(nodeID1,nodeID4));
	graph->add_edge(e);


	/**************************************************
	 * the graph is a straight line of nodes 0 - 999  *
	 * with a directed edge from 1 to 4				  *
	 * so degree is three (at node 1) 				  *
	 **************************************************/
	BOOST_CHECK_EQUAL(stats_calc_.calculate_degree(history->get_newest().world_information_ptr()), 3);

	//add another directed edge to check that only sources of directed edges are taken into account
	boost::shared_ptr<RobotIdentifier> nodeID20 = IDs[20];
	//create edge
	boost::shared_ptr<DirectedEdge> e2 (new DirectedEdge(nodeID20,nodeID4));
	graph->add_edge(e2);

	/**************************************************
	 * the graph is a straight line of nodes 0 - 999  *
	 * with a directed edge from 1 to 4	and			  *
	 * with a directed edge from 20 to 4			  *
	 * so degree is three (at node 1 and 20)		  *
	 * (node 4 has still degree 2)					  *
	 **************************************************/
	BOOST_CHECK_EQUAL(stats_calc_.calculate_degree(history->get_newest().world_information_ptr()), 3);


	boost::shared_ptr<RobotData> node7 = nodes[7];
	boost::shared_ptr<RobotData> node10 = nodes[10];
	boost::shared_ptr<RobotData> node12 = nodes[12];
	const std::vector<boost::shared_ptr<EdgeIdentifier> > null;
	/**************************************************
	 * the graph is a straight line of nodes 0 - 999  *
	 * with a directed edge from 1 to 4	and			  *
	 * with a directed edge from 20 to 4			  *
	 * node 1 needs 1 hop to node 4	(directed edge)	  *
	 * but node 4 needs 3 hops (line-edges)			  *
	 **************************************************/
	BOOST_CHECK_EQUAL(stats_calc_.calculate_hop_distance(history->get_newest().world_information_ptr(), IDs[1], IDs[4], null),1);
	BOOST_CHECK_EQUAL(stats_calc_.calculate_hop_distance(history->get_newest().world_information_ptr(), IDs[4], IDs[1], null),3);

	//ignore edge 1->4
	std::vector<boost::shared_ptr<EdgeIdentifier> > ignore_one_edge;
	ignore_one_edge.push_back(boost::dynamic_pointer_cast<EdgeIdentifier>(e->id()));
	/**************************************************
	 * the graph is a straight line of nodes 0 - 999  *
	 * with a directed edge from 1 to 4	and			  *
	 * with a directed edge from 20 to 4			  *
	 * node 1 needs 3 hop to node 4	with the directed *
	 * edge being ignored							  *
	 **************************************************/
	BOOST_CHECK_EQUAL(stats_calc_.calculate_hop_distance(history->get_newest().world_information_ptr(), IDs[1], IDs[4], ignore_one_edge),3);

	//node 7 gets same color as node 8
	(nodes[7]->marker_information()).remove_data(":color");
	(nodes[7]->marker_information()).add_data(":color",8.0);

	//nodes 10 and 12 get same color as node 11
	(nodes[10]->marker_information()).remove_data(":color");
	(nodes[10]->marker_information()).add_data(":color",11.0);
	(nodes[12]->marker_information()).remove_data(":color");
	(nodes[12]->marker_information()).add_data(":color",11.0);

	/**************************************************
	 * the graph is a straight line of nodes 0 - 999  *
	 * with a directed edge from 1 to 4	and			  *
	 * with a directed edge from 20 to 4			  *
	 * each node has its own color but nodes 7 and 8  *
	 * have the same color and nodes 10, 11 and 12	  *
	 * have also only one color						  *
	 * so the max number of defects a node has is two *
	 * (node 11 has defects with neighbours 10 and 12)*
	 * and the total number of defects is three		  *
	 * (defects between 7<->8 , 10<->11, 11<->12)	  *
	 **************************************************/
	BOOST_CHECK_EQUAL(stats_calc_.calculate_maximal_defect(history->get_newest().world_information_ptr()),2);
	BOOST_CHECK_EQUAL(stats_calc_.calculate_total_defects(history->get_newest().world_information_ptr()),3);
}

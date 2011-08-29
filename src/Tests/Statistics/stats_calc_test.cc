#include <iostream>
#include <iomanip>
#include <ctime>
#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>


#include "../../Statistics/stats_calc.h"

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Model/undirected_edge.h"
#include "../../Statistics/stats_calc.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../Fixtures/simple_world_fixture.h"

const unsigned kCopyCount = 127;

BOOST_AUTO_TEST_CASE(stats_calc_test) {

	boost::shared_ptr<WorldInformation> graph (new WorldInformation());

	std::vector<boost::shared_ptr<RobotIdentifier> > IDs;

	boost::shared_ptr<Vector3d> pos(new Vector3d());
	pos->insert_element(kXCoord,0.0);
	pos->insert_element(kYCoord,0.0);
	pos->insert_element(kZCoord,0.0);

	//create 1000 Nodes and NodeDatas
	for(unsigned i = 0; i < 1000; i++) {
		boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
		IDs.push_back(id);
		//TODO: undefined behavior here, since robot is deleted after each forloop run.
		boost::shared_ptr<Robot> node(new SimpleRobot(id));
		boost::shared_ptr<RobotData> nodeData(new RobotData(id, pos, *node));

		nodeData->set_color(i);
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

		RobotData& rd1 = graph->get_according_robot_data(e->getRobot1());
		RobotData& rd2 = graph->get_according_robot_data(e->getRobot2());

		// add requested edge to world_information and to adjacency list of robots
		rd1.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e->id()));
		rd2.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e->id()));

		graph->add_edge(e);
	}

	//get StatsCalc class
	StatsCalc stats_calc_;

	//degree has to be 2
	BOOST_CHECK_EQUAL(stats_calc_.calculate_degree(graph), 2);

	//max number of defects is 0
	BOOST_CHECK_EQUAL(stats_calc_.calculate_maximal_defect(graph),0);
	//total number of defects is 0
	BOOST_CHECK_EQUAL(stats_calc_.calculate_total_defects(graph),0);

	//add one directed edge to the second node
	boost::shared_ptr<RobotIdentifier> nodeID1 = IDs[1];
	boost::shared_ptr<RobotIdentifier> nodeID2 = IDs[4];

	//create edge
	boost::shared_ptr<DirectedEdge> e (new DirectedEdge(nodeID1,nodeID2));

	RobotData& rd1 = graph->get_according_robot_data(e->source());

	// add requested edge to world_information and to adjacency list of robots
	rd1.add_edge(boost::dynamic_pointer_cast<EdgeIdentifier>(e->id()));

	graph->add_edge(e);

	//degree has to be 3
	BOOST_CHECK_EQUAL(stats_calc_.calculate_degree(graph), 3);


	boost::shared_ptr<RobotData> node7 = nodes[7];

	boost::shared_ptr<RobotData> node10 = nodes[10];
	boost::shared_ptr<RobotData> node12 = nodes[12];

	const std::vector<boost::shared_ptr<EdgeIdentifier> > null;
	//node1 is 1 hop away from node 4
	BOOST_CHECK_EQUAL(stats_calc_.calculate_hop_distance(graph, IDs[1], IDs[4], null),1);

	//ignore edge 1->4
	//so hop distance is 3
	std::vector<boost::shared_ptr<EdgeIdentifier> > ignore_one_edge;
	ignore_one_edge.push_back(boost::dynamic_pointer_cast<EdgeIdentifier>(e->id()));
	BOOST_CHECK_EQUAL(stats_calc_.calculate_hop_distance(graph, IDs[1], IDs[4], ignore_one_edge),3);

	node7->set_color(8);

	node10->set_color(11);
	node12->set_color(11);

	//max number of defects is 2
	BOOST_CHECK_EQUAL(stats_calc_.calculate_maximal_defect(graph),2);
	//total number of defects is 3
	BOOST_CHECK_EQUAL(stats_calc_.calculate_total_defects(graph),3);
}

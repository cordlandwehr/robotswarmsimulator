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

	/*boost::shared_ptr<WorldInformation> graph (new WorldInformation());

	boost::shared_ptr<Vector3d> pos(new Vector3d());
	pos->insert_element(kXCoord,0.0);
	pos->insert_element(kYCoord,0.0);
	pos->insert_element(kZCoord,0.0);

	//create 1000 Nodes and NodeDatas
	for(unsigned i = 0; i < 1000; i++) {
		boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
		//TODO: undefined behavior here, since robot is deleted after each forloop run.
		boost::shared_ptr<Robot> node(new SimpleRobot(id));
		boost::shared_ptr<RobotData> nodeData(new RobotData(id, pos, *node));
		graph->add_robot_data(nodeData);
	}

	BOOST_CHECK_EQUAL(graph->robot_data().size(), 1000);

	//create edge between consecutive nodes (gives a line)
	const std::vector<boost::shared_ptr<RobotData> >& nodes = graph->robot_data();
	for(int i=1; i<nodes.size();i++){
		boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
		boost::shared_ptr<RobotData> node1 = nodes[i-1];
		boost::shared_ptr<RobotData> node2 = nodes[i];

		//create edge
		boost::shared_ptr<Edge> e (new UndirectedEdge(id,pos,node1,node2));
		//add edge to nodes
		node1->add_edge(e);
		node2->add_edge(e);
	}

	//get StatsCalc class
	StatsCalc stats_calc_;

	//degree has to be 2
	BOOST_CHECK_EQUAL(stats_calc_.calculateDegree(nodes), 2);


	//add one directed edge to the second node
	boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(nodes.size()));
	boost::shared_ptr<RobotData> node1 = nodes[1];
	boost::shared_ptr<RobotData> node2 = nodes[4];

	//create edge
	boost::shared_ptr<Edge> e (new Edge(id,pos,node1,node2));
	//add edge to nodes
	node1->add_edge(e);

	//degree has to be 3
	BOOST_CHECK_EQUAL(stats_calc_.calculateDegree(nodes), 3);*/
}

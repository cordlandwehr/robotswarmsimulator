#include "layouter.h"

#include <boost/foreach.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>

#include "../Utilities/vector_arithmetics.h"
#include "../Model/robot_data.h"

boost::shared_ptr<BoostGraph> Layouter::transform_into_boost_graph(const WorldInformation& world_information){

	boost::shared_ptr<BoostGraph> g (new BoostGraph());

	BOOST_FOREACH(const boost::shared_ptr<RobotData>& robot, world_information.robot_data()) {
		boost::graph_traits<BoostGraph>::vertex_descriptor  descriptor = boost::add_vertex(*g);
		robot->set_boost_vertex_descriptor(descriptor);
		put(vertex_robot, *g, descriptor, robot);


		Vector3d position = robot->position();
		boost::square_topology<>::point_type new_position;
		new_position[0] = position[0];
		new_position[1] = position[1];

		put(vertex_position, *g, descriptor, new_position);
	}

	typedef const std::map<std::size_t, boost::shared_ptr<Edge> >::value_type EdgeItem;


	BOOST_FOREACH(EdgeItem& edge, world_information.edges()){
		const RobotData& robot1 = world_information.get_according_robot_data(edge.second->getRobot1());
		const RobotData& robot2 = world_information.get_according_robot_data(edge.second->getRobot2());

		boost::add_edge(robot1.get_boost_vertex_descriptor(), robot2.get_boost_vertex_descriptor(), *g);
	}


	return g;
}


void Layouter::apply_positions(const BoostGraph& g){

	BGL_FORALL_VERTICES_T(v, g, BoostGraph) {
		boost::shared_ptr<RobotData> robot = get(vertex_robot, g)[v];
		boost::square_topology<>::point_type position = get(vertex_position, g)[v];
		boost::shared_ptr<Vector3d> vec_position(new Vector3d());
		(*vec_position)[0] = position[0];
		(*vec_position)[1] = position[1];
		(*vec_position)[2] = 0;

		robot->set_position(vec_position);
	}
}


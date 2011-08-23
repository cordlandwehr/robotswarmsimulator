#include "force_directed_layouter.h"

#include <boost/foreach.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/circle_layout.hpp>

#include "../Model/robot_data.h"

ForceDirectedLayouter::ForceDirectedLayouter(){

}

std::string ForceDirectedLayouter::get_algorithm_id() const{
	return "RANDOM_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
ForceDirectedLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {

	BoostGraph g;

	BOOST_FOREACH(const boost::shared_ptr<RobotData>& robot, world_information->robot_data()) {
		boost::graph_traits<BoostGraph>::vertex_descriptor  descriptor = boost::add_vertex(g);
		robot->set_boost_vertex_descriptor(descriptor);
		put(vertex_robot, g, descriptor, robot);


		Vector3d position = robot->position();
		boost::square_topology<>::point_type new_position;
		new_position[0] = position[0];
		new_position[1] = position[1];

		put(vertex_position, g, descriptor, new_position);
	}

	typedef std::map<std::size_t, boost::shared_ptr<Edge> >::value_type EdgeItem;


	BOOST_FOREACH(EdgeItem& edge, world_information->edges()){
		RobotData& robot1 = world_information->get_according_robot_data(edge.second->getRobot1());
		RobotData& robot2 = world_information->get_according_robot_data(edge.second->getRobot2());

		boost::add_edge(robot1.get_boost_vertex_descriptor(), robot2.get_boost_vertex_descriptor(), g);
	}

	boost::minstd_rand gen;
	boost::rectangle_topology<> rect_top(gen, 0, 0, 50, 50);
	boost::random_graph_layout(g, get(vertex_position, g), rect_top);
	boost::circle_graph_layout(g, get(vertex_position, g), 20.0);

	  typedef boost::square_topology<> Topology;
	  Topology topology(gen, 1000.0);
	fruchterman_reingold_force_directed_layout(g, get(vertex_position, g), topology);

	BGL_FORALL_VERTICES_T(v, g, BoostGraph) {
		boost::shared_ptr<RobotData> robot = get(vertex_robot, g)[v];
		boost::square_topology<>::point_type position = get(vertex_position, g)[v];
		boost::shared_ptr<Vector3d> vec_position(new Vector3d());
		(*vec_position)[0] = position[0];
		(*vec_position)[1] = position[1];
		(*vec_position)[2] = 0;

		robot->set_position(vec_position);
	}




	//boost::minstd_rand gen;
	//typedef boost::square_topology<> Topology;
	//Topology topology(gen, 50.0);
	//std::vector<Topology::point_difference_type> displacements(world_information->robot_data().size());
//	boost::rectangle_topology<> rect_top(gen, 0, 0, 50, 50);
//
//	boost::lvalue_position_property_map map;
//
//	std::vector<boost::shared_ptr<RobotData> > nodes =world_information->robot_data();
//
//
//	std::vector<boost::shared_ptr<RobotData> >::iterator myIterator;
//	for(myIterator = nodes.begin();
//			myIterator != nodes.end();
//			myIterator++)
//	{
//		boost::rectangle_topology<>::point_type point;
//		point[0] = 0;
//		point[1] = 0;
//
//		map.insert(std::make_pair((*myIterator), point));
//
//	}
//
//	  boost::associative_property_map< boost::lvalue_position_property_map >
//	    wrapped_map(map);
//
//	boost::random_graph_layout(*world_information, wrapped_map, rect_top);
//
//	boost::fruchterman_reingold_force_directed_layout(*world_information, wrapped_map, rect_top);
//	  boost::fruchterman_reingold_force_directed_layout
//	    (*world_information,
//	     *world_information,
//	     topology,
//	     boost::square_distance_attractive_force(),
//	     boost::square_distance_repulsive_force(),
//	     boost::all_force_pairs(),
//	     boost::linear_cooling<double>(100),
//	     boost::make_iterator_property_map(displacements.begin(),
//	                                *world_information,
//	                                Topology::point_difference_type()));

	std::set< boost::shared_ptr<Request> > result;
	return result;
}

#include "boost_graph_wrapper.h"

namespace boost {

	typename graph_traits< WorldInformation >::vertex_descriptor
		  source(typename graph_traits<WorldInformation >::edge_descriptor e,
				 const WorldInformation& g)
		  {
			  WorldInformation& graph = const_cast<WorldInformation&>(g);
			  RobotData& r = graph.get_according_robot_data(e.getRobot1());
			  shared_ptr<RobotData> rp(&r);
			  return rp;
		  }

	typename graph_traits< WorldInformation >::vertex_descriptor
	target(typename graph_traits< WorldInformation >::edge_descriptor e,
		   const WorldInformation& g)
	{
		  WorldInformation& graph = const_cast<WorldInformation&>(g);
		  RobotData& r = graph.get_according_robot_data(e.getRobot2());
		  shared_ptr<RobotData> rp(&r);
		  return rp;
	}

	typename graph_traits< WorldInformation >::vertices_size_type
	num_vertices(const WorldInformation& g)
	{
	  return g.robot_data().size();
	}

	void put(WorldInformation& map, boost::graph_traits<WorldInformation>::vertex_descriptor& key, boost::rectangle_topology<>::point_type value ){

		//std::cout << value[0] << " " << value[1] << std::endl;

		  boost::shared_ptr<Vector3d> v (new Vector3d());
		  (*v)[0] = value[0];
		  (*v)[1] = value[1];
		  (*v)[2] = 0.0;

		  key->set_position(v);
	}

	std::pair<
		  typename graph_traits< WorldInformation >::vertex_iterator,
		  typename graph_traits< WorldInformation >::vertex_iterator >
	vertices(const WorldInformation& g)
	{
		const std::vector<boost::shared_ptr<RobotData> >& robots = g.robot_data();

		typename graph_traits< WorldInformation >::vertex_iterator begin = robots.begin();
		typename graph_traits< WorldInformation >::vertex_iterator end = robots.end();

	  return std::make_pair( begin, end);
	}

}

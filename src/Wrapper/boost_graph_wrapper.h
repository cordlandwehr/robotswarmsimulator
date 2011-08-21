//
//  boost_graph_wrapper.h
//  Robot Swarm Simulator
//
//  Created by xynodes on 20.08.11.
//

#ifndef BOOST_GRAPH_WRAPPER_H_
#define BOOST_GRAPH_WRAPPER_H_

#include <boost/config.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/topology.hpp>

#include <Model/world_information.h>
#include <Model/robot_data.h>
#include <Model/edge.h>


namespace boost {
	template <>
	struct graph_traits< WorldInformation > {
		//Adaptation for VertexListGraphConcept
		typedef boost::shared_ptr<RobotData> vertex_descriptor;
		typedef Edge edge_descriptor;

		// iterator typedefs...
		typedef std::vector<edge_descriptor> ::iterator out_edge_iterator;
		typedef std::vector<boost::shared_ptr<RobotData> >::const_iterator vertex_iterator;

		typedef directed_tag directed_category;
		typedef allow_parallel_edge_tag edge_parallel_category;
		typedef  vertex_list_graph_tag  traversal_category;

		typedef std::vector<vertex_descriptor>::size_type vertices_size_type;
		typedef int edges_size_type;
		typedef int degree_size_type;


	};



	typename graph_traits< WorldInformation >::vertex_descriptor
	source(typename graph_traits<WorldInformation >::edge_descriptor e,
		 const WorldInformation& g);


	  typename graph_traits< WorldInformation >::vertex_descriptor
	  target(typename graph_traits< WorldInformation >::edge_descriptor e,
	         const WorldInformation& g);


	  std::pair<
	  	  typename graph_traits< WorldInformation >::vertex_iterator,
	  	  typename graph_traits< WorldInformation >::vertex_iterator >
	  vertices(const WorldInformation& g);

	  typename graph_traits< WorldInformation >::vertices_size_type
	  num_vertices(const WorldInformation& g);


	  ////////////////////////////////////////////////
	  //  Adaptation for WritablePropertyMapConcept
	  /////////////////////////////////////////////////
	  template <>
	  struct property_traits<WorldInformation>{
		typedef graph_traits< WorldInformation >::vertex_descriptor key_type;
		typedef writable_property_map_tag category;
		typedef rectangle_topology<>::point_type value_type;
	  };

	  void put(WorldInformation& map, boost::graph_traits<WorldInformation>::vertex_descriptor& key, boost::rectangle_topology<>::point_type value );

}


#endif // BOOST_GRAPH_WRAPPER_H_

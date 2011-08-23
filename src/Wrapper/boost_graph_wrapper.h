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
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/topology.hpp>


//namespace boost {
//	struct vertex_and_edge_list_graph_traversal_category :
//	  public virtual edge_list_graph_tag,
//	  public virtual vertex_list_graph_tag { };
//
//
//
//	template <>
//	struct graph_traits< WorldInformation > {
//		//Adaptation for VertexListGraphConcept
//		typedef boost::shared_ptr<RobotData> vertex_descriptor;
//		typedef boost::shared_ptr<Edge> edge_descriptor;
//
//		// iterator typedefs...
//		typedef std::vector<edge_descriptor> ::iterator out_edge_iterator;
//		typedef std::vector<boost::shared_ptr<RobotData> >::const_iterator vertex_iterator;
//		typedef std::vector<boost::shared_ptr<Edge> >::const_iterator edge_iterator;
//
////	    class edge_iterator
////	      : public iterator_facade<edge_iterator,
////	        						edge_descriptor,
////	                               bidirectional_traversal_tag,
////	                               const edge_descriptor,
////	                               const std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator>
////	    {
////	    public:
////	    	  edge_iterator(std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator iter = 0)
////	        : iter(iter) {}
////
////	    private:
////	      const edge_descriptor& dereference() const { return (*iter).second; }
////
////	      bool equal(const edge_iterator& other) const
////	      { return (*iter).second == (*other.iter).second; }
////
////	      void increment() {
////	    	  iter++;
////	      }
////	      void decrement() {
////	    	  iter--;
////	      }
////
////	      std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator iter;
////
////	      friend class iterator_core_access;
////	    };
//
//
//
//
//		typedef directed_tag directed_category;
//		typedef allow_parallel_edge_tag edge_parallel_category;
//		typedef  vertex_and_edge_list_graph_traversal_category  traversal_category;
//
//		typedef std::vector<vertex_descriptor>::size_type vertices_size_type;
//		typedef unsigned int edges_size_type;
//		typedef int degree_size_type;
//
//
//	};
//
//
//
//	typename graph_traits< WorldInformation >::vertex_descriptor
//	source(typename graph_traits<WorldInformation >::edge_descriptor e,
//		 const WorldInformation& g);
//
//
//	  typename graph_traits< WorldInformation >::vertex_descriptor
//	  target(typename graph_traits< WorldInformation >::edge_descriptor e,
//	         const WorldInformation& g);
//
//
//	  std::pair<
//	  	  typename graph_traits< WorldInformation >::vertex_iterator,
//	  	  typename graph_traits< WorldInformation >::vertex_iterator >
//	  vertices(const WorldInformation& g);
//
//	  typename graph_traits< WorldInformation >::vertices_size_type
//	  num_vertices(const WorldInformation& g);
//
//	  std::pair<
//		    typename graph_traits< WorldInformation >::edge_iterator,
//		    typename graph_traits< WorldInformation >::edge_iterator >
//	  edges(const WorldInformation& g);
//
//	  typename graph_traits< WorldInformation >::edges_size_type
//	  num_edges(const WorldInformation& g);
//
//
//	  ////////////////////////////////////////////////
//	  //  Adaptation for WritablePropertyMapConcept
//	  /////////////////////////////////////////////////
//	  typedef std::map<boost::graph_traits<WorldInformation>::vertex_descriptor, boost::rectangle_topology<>::point_type> lvalue_position_property_map;
//
//	  struct my_property_map_tag:
//		  virtual lvalue_property_map_tag{};
//
//	  template <>
//	  struct property_traits<lvalue_position_property_map>{
//		typedef graph_traits< WorldInformation >::vertex_descriptor key_type;
//		typedef my_property_map_tag category;
//		typedef rectangle_topology<>::point_type value_type;
//		typedef rectangle_topology<>::point_type& reference;
//	  };
//
//	  void put(lvalue_position_property_map& map, property_traits<lvalue_position_property_map>::key_type& key, property_traits<lvalue_position_property_map>::value_type value);
//
//	  property_traits<lvalue_position_property_map>::reference get(lvalue_position_property_map map, property_traits<lvalue_position_property_map>::key_type key);
//
//	  void put(WorldInformation& map, boost::graph_traits<WorldInformation>::vertex_descriptor& key, boost::rectangle_topology<>::point_type value );
//	  void put(WorldInformation& map, boost::shared_ptr<RobotData>& key, boost::convex_topology<2u>::point value);
//	  boost::rectangle_topology<>::point_type& get(const WorldInformation& pmap, boost::shared_ptr<RobotData>& key);
//}

enum vertex_position_t { vertex_position };
enum vertex_robot_t {vertex_robot };
namespace boost { BOOST_INSTALL_PROPERTY(vertex, position); }
namespace boost { BOOST_INSTALL_PROPERTY(vertex, robot); }
typedef boost::square_topology<>::point_type point;

class RobotData;

typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS,
                        // Vertex properties
                        boost::property<boost::vertex_index_t, int,
                        boost::property<vertex_position_t, point,
                        boost::property<vertex_robot_t, boost::shared_ptr<RobotData> > > >,
                        // Edge properties
                        boost::property<boost::edge_weight_t, double> > BoostGraph;


#endif // BOOST_GRAPH_WRAPPER_H_

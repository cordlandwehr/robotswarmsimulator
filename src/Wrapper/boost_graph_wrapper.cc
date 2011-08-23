#include "boost_graph_wrapper.h"

//namespace boost {
//
//	typename graph_traits< WorldInformation >::vertex_descriptor
//		  source(typename graph_traits<WorldInformation >::edge_descriptor e,
//				 const WorldInformation& g)
//		  {
//			  WorldInformation& graph = const_cast<WorldInformation&>(g);
//			  RobotData& r = graph.get_according_robot_data(e->getRobot1());
//			  shared_ptr<RobotData> rp(&r);
//			  return rp;
//		  }
//
//	typename graph_traits< WorldInformation >::vertex_descriptor
//	target(typename graph_traits< WorldInformation >::edge_descriptor e,
//		   const WorldInformation& g)
//	{
//		  WorldInformation& graph = const_cast<WorldInformation&>(g);
//		  RobotData& r = graph.get_according_robot_data(e->getRobot2());
//		  shared_ptr<RobotData> rp(&r);
//		  return rp;
//	}
//
//	typename graph_traits< WorldInformation >::vertices_size_type
//	num_vertices(const WorldInformation& g)
//	{
//	  return g.robot_data().size();
//	}
//
//	std::pair<
//		  typename graph_traits< WorldInformation >::vertex_iterator,
//		  typename graph_traits< WorldInformation >::vertex_iterator >
//	vertices(const WorldInformation& g)
//	{
//		const std::vector<boost::shared_ptr<RobotData> >& robots = g.robot_data();
//
//		typename graph_traits< WorldInformation >::vertex_iterator begin = robots.begin();
//		typename graph_traits< WorldInformation >::vertex_iterator end = robots.end();
//
//	  return std::make_pair( begin, end);
//	}
//
//	 std::pair<
//	    typename graph_traits< WorldInformation >::edge_iterator,
//	    typename graph_traits< WorldInformation >::edge_iterator >
//	  edges(const WorldInformation& g)
//	  {
////		 std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator begin_tmp = g.edges().begin();
////		 std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator end_tmp = g.edges().end();
////
////		 typename graph_traits< WorldInformation >::edge_iterator begin(begin_tmp);
////		typename graph_traits< WorldInformation >::edge_iterator end (end_tmp);
////
////		return std::make_pair( begin, end);
//
//		 std::vector<boost::shared_ptr<Edge> > edges;
//
//
//		 std::map<std::size_t, boost::shared_ptr<Edge> >::const_iterator myIterator;
//
//		 std::map<std::size_t, boost::shared_ptr<Edge> > original_edges = g.edges();
//
//		 for(myIterator = original_edges.begin();
//				 myIterator != original_edges.end();
//				 myIterator++)
//		 {
//			 //edges.push_pack(myIterator->second);
//			 edges.push_back((*myIterator).second);
//
//		 }
//
//
//
//		  return std::make_pair(edges.begin(), edges.end());
//	  }
//
//	  typename graph_traits< WorldInformation >::edges_size_type
//	  num_edges(const WorldInformation& g){
//		  return g.edges().size();
//	  }
//
//	  void put(lvalue_position_property_map& map, property_traits<lvalue_position_property_map>::key_type& key, property_traits<lvalue_position_property_map>::value_type value){
//		  map[key] = value;
//	  }
//	  property_traits<lvalue_position_property_map>::reference get(lvalue_position_property_map map, property_traits<lvalue_position_property_map>::key_type key){
//		  return map[key];
//	  }
//
//	  void put(WorldInformation& map, boost::graph_traits<WorldInformation>::vertex_descriptor& key, boost::rectangle_topology<>::point_type& value ){
//
//		//std::cout << value[0] << " " << value[1] << std::endl;
//
//		  boost::shared_ptr<Vector3d> v (new Vector3d());
//		  (*v)[0] = value[0];
//		  (*v)[1] = value[1];
//		  (*v)[2] = 0.0;
//
//		  key->set_position(v);
//	}
//
//	  void put(WorldInformation& map, boost::shared_ptr<RobotData>& key, boost::convex_topology<2u>::point value){
//		  boost::shared_ptr<Vector3d> v (new Vector3d());
//		  (*v)[0] = value[0];
//		  (*v)[1] = value[1];
//		  (*v)[2] = 0.0;
//
//		  key->set_position(v);
//	  }
//
//	boost::rectangle_topology<>::point_type& get(const WorldInformation& pmap, boost::shared_ptr<RobotData>& key){
//		Vector3d position = key->position();
//
//		boost::rectangle_topology<>::point_type point;
//
//		point[0] = position[0];
//		point[1] = position[1];
//
//		return point;
//	}
//
//
//}

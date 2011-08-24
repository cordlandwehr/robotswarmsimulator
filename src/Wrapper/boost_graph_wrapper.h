//
//  boost_graph_wrapper.h
//  Robot Swarm Simulator
//
//  Created by xynodes on 20.08.11.
//

#ifndef BOOST_GRAPH_WRAPPER_H_
#define BOOST_GRAPH_WRAPPER_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/topology.hpp>

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

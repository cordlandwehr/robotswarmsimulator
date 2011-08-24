//
//  random_layouter.cc
//  Robot Swarm Simulator
//
//  Created by xynodes on 24.08.11.
//

#include "random_layouter.h"

#include <boost/graph/random_layout.hpp>

#include "../Model/robot_data.h"

RandomLayouter::RandomLayouter(){

}

std::string RandomLayouter::get_algorithm_id() const{
	return "RANDOM_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
RandomLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {
	boost::shared_ptr<BoostGraph> g = Layouter::transform_into_boost_graph(*world_information);

	boost::minstd_rand gen;
	boost::rectangle_topology<> rect_top(gen, 0, 0, 50, 50);
	boost::random_graph_layout(*g, get(vertex_position, *g), rect_top);

	Layouter::apply_positions(*g);

	std::set< boost::shared_ptr<Request> > result;
	return result;
}

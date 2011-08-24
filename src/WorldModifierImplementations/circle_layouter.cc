//
//  circle_layouter.cc
//  Robot Swarm Simulator
//
//  Created by xynodes on 24.08.11.
//

#include "circle_layouter.h"

#include <boost/graph/circle_layout.hpp>

CircleLayouter::CircleLayouter(){

}

std::string CircleLayouter::get_algorithm_id() const{
	return "CIRCLE_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
CircleLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {
	boost::shared_ptr<BoostGraph> g = Layouter::transform_into_boost_graph(*world_information);
	boost::circle_graph_layout(*g, get(vertex_position, *g), 20.0);
	Layouter::apply_positions(*g);

	std::set< boost::shared_ptr<Request> > result;
	return result;
}

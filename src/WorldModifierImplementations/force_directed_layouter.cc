//
//  force_directed_layouter.cc
//  Robot Swarm Simulator
//
//  Created by xynodes on 24.08.11.
//

#include "force_directed_layouter.h"

#include <boost/random/linear_congruential.hpp>
#include <boost/graph/fruchterman_reingold.hpp>




ForceDirectedLayouter::ForceDirectedLayouter(){

}

std::string ForceDirectedLayouter::get_algorithm_id() const{
	return "RANDOM_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
ForceDirectedLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {

	boost::shared_ptr<BoostGraph> g = Layouter::transform_into_boost_graph(*world_information);

	boost::minstd_rand gen;
	int n = world_information->robot_data().size();
	double sideLength = 200*sqrt(n);
	boost::rectangle_topology<> rect_top(gen, 0, 0, sideLength, sideLength);

	fruchterman_reingold_force_directed_layout(*g, get(vertex_position, *g), rect_top);

	Layouter::apply_positions(*g);

	std::set< boost::shared_ptr<Request> > result;
	return result;
}

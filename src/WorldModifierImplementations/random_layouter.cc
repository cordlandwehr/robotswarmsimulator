#include "random_layouter.h"

RandomLayouter::RandomLayouter(){

}

std::string RandomLayouter::get_algorithm_id() const{
	return "RANDOM_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
RandomLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {
	boost::minstd_rand gen;
	typedef boost::square_topology<> Topology;
	Topology topology(gen, 50.0);
	std::vector<Topology::point_difference_type> displacements(world_information->robot_data().size());
	boost::rectangle_topology<> rect_top(gen, 0, 0, 50, 50);

	boost::random_graph_layout(*world_information, *world_information, rect_top);

	std::set< boost::shared_ptr<Request> > result;
	return result;
}

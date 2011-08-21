#include "circle_layouter.h"

CircleLayouter::CircleLayouter(){

}

std::string CircleLayouter::get_algorithm_id() const{
	return "CIRCLE_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
CircleLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {
	//TODO (xynodes): call circle_layout

	WorldInformation& wi = *world_information;

	boost::minstd_rand gen;
	typedef boost::square_topology<> Topology;
	Topology topology(gen, 50.0);
	std::vector<Topology::point_difference_type> displacements(wi.robot_data().size());
	boost::rectangle_topology<> rect_top(gen, 0, 0, 50, 50);

	//std::cout << "starting layout .." << std::endl;

	//boost::circle_graph_layout(wi, wi, 10.0);
	boost::random_graph_layout(wi, wi, rect_top);

	//std::cout << "layout finished .." << std::endl;


	std::set< boost::shared_ptr<Request> > result;
	return result;
}

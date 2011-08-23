#include "force_directed_layouter.h"

ForceDirectedLayouter::ForceDirectedLayouter(){

}

std::string ForceDirectedLayouter::get_algorithm_id() const{
	return "RANDOM_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
ForceDirectedLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {
	boost::minstd_rand gen;
	//typedef boost::square_topology<> Topology;
	//Topology topology(gen, 50.0);
	//std::vector<Topology::point_difference_type> displacements(world_information->robot_data().size());
	boost::rectangle_topology<> rect_top(gen, 0, 0, 50, 50);

	boost::lvalue_position_property_map map;

	std::vector<boost::shared_ptr<RobotData> > nodes =world_information->robot_data();


	std::vector<boost::shared_ptr<RobotData> >::iterator myIterator;
	for(myIterator = nodes.begin();
			myIterator != nodes.end();
			myIterator++)
	{
		boost::rectangle_topology<>::point_type point;
		point[0] = 0;
		point[1] = 0;

		map.insert(std::make_pair((*myIterator), point));

	}

	  boost::associative_property_map< boost::lvalue_position_property_map >
	    wrapped_map(map);

	boost::random_graph_layout(*world_information, wrapped_map, rect_top);

	boost::fruchterman_reingold_force_directed_layout(*world_information, wrapped_map, rect_top);
//	  boost::fruchterman_reingold_force_directed_layout
//	    (*world_information,
//	     *world_information,
//	     topology,
//	     boost::square_distance_attractive_force(),
//	     boost::square_distance_repulsive_force(),
//	     boost::all_force_pairs(),
//	     boost::linear_cooling<double>(100),
//	     boost::make_iterator_property_map(displacements.begin(),
//	                                *world_information,
//	                                Topology::point_difference_type()));

	std::set< boost::shared_ptr<Request> > result;
	return result;
}

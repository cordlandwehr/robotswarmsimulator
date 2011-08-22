#include "circle_layouter.h"

CircleLayouter::CircleLayouter(){

}

std::string CircleLayouter::get_algorithm_id() const{
	return "CIRCLE_LAYOUTER";
}

std::set< boost::shared_ptr<Request> >
CircleLayouter::compute(const boost::shared_ptr<WorldInformation> &world_information) {
    const double pi = boost::math::constants::pi<double>();


	std::vector<boost::shared_ptr<RobotData> > nodes =world_information->robot_data();

    int n = nodes.size();
    int i = 0;
    double two_pi_over_n = 2. * pi / n;
    double radius = 20.0;

	std::vector<boost::shared_ptr<RobotData> >::iterator myIterator;
	for(myIterator = nodes.begin();
			myIterator != nodes.end();
			myIterator++)
	{
	    Vector3d position = (*myIterator)->position();

	    position[0] = radius * std::cos(i * two_pi_over_n);
	    position[1] = radius * std::sin(i * two_pi_over_n);
	    position[2] = 0;

	    boost::shared_ptr<Vector3d> position_ptr(new Vector3d(position));

	    (*myIterator)->set_position(position_ptr);
	    i++;
	}







    /*BGL_FORALL_VERTICES_T(v, g, WorldInformation) {
      position[v][0] = radius * std::cos(i * two_pi_over_n);
      position[v][1] = radius * std::sin(i * two_pi_over_n);
      ++i;
    }*/


	std::set< boost::shared_ptr<Request> > result;
	return result;
}

/*
 * full_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#include "full_view.h"
#include "../Model/world_information.h"
#include "../Model/identifier.h"
#include "../Model/marker_identifier.h"
#include "../Model/robot_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/world_object.h"
#include "../Model/obstacle.h"


#include <boost/foreach.hpp>


namespace {
	template<typename S, typename T>
	std::vector<boost::shared_ptr<S> > convert_to_set(const std::vector<boost::shared_ptr<T> >& vec) {
		std::vector<boost::shared_ptr<S> > set;
		BOOST_FOREACH(const boost::shared_ptr<T>& t, vec) {
			set.push_back(boost::static_pointer_cast<S>(t->id()));
		}
		return set;
	}
}

FullView::FullView() {


}

FullView::~FullView() {

}

std::vector<View::RobotRef> FullView::get_visible_robots(const RobotData& robot) const {
	std::vector<View::RobotRef> set = convert_to_set<RobotIdentifier, RobotData>(world_information().robot_data());
	//remove self
	set.erase(std::find(set.begin(), set.end(), boost::static_pointer_cast<RobotIdentifier>(robot.id())));
	return set;
}
std::vector<View::ObstacleRef> FullView::get_visible_obstacles(const RobotData& robot) const {
	return convert_to_set<ObstacleIdentifier, Obstacle>(world_information().obstacles());
}
std::vector<View::MarkerRef> FullView::get_visible_markers(const RobotData& robot) const {
	return convert_to_set<MarkerIdentifier, WorldObject>(world_information().markers());
}

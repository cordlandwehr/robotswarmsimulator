/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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

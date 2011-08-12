/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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
 * spheric_view.h
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#ifndef SPHERIC_VIEW_H_
#define SPHERIC_VIEW_H_

#include "octree_view.h"
#include "../Model/identifier.h"

/**
 * \class SphericView
 * \brief Basic spherical view model.
 *
 * Provides implementations of getVisible... methods. To efficiently implement them
 * the classes uses an Octree as internal data structure.
 *
 * Assigning this class to a Robot corresponds to a "see all objects in a given radius" model.
 *
 */

class SphericView : public virtual OctreeView {
public:
	SphericView(double view_radius);
	virtual ~SphericView();
	void init(const boost::shared_ptr<WorldInformation>& world_information );

	virtual const double get_view_radius() const;

protected:
	virtual std::vector<RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::vector<ObstacleRef> get_visible_obstacles(const RobotData& robot) const;
	virtual std::vector<MarkerRef> get_visible_markers(const RobotData& robot) const;

	double view_radius() const;

private:
	double view_radius_;
};

#endif /* SPHERIC_VIEW_H_ */

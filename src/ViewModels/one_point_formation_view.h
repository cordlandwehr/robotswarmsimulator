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
 * one_point_formation_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef ONE_POINT_FORMATION_VIEW_H_
#define ONE_POINT_FORMATION_VIEW_H_

#include "../Views/velocity_view.h"
#include "../Views/spheric_view.h"
#include "../Views/acceleration_view.h"
#include "../Views/position_view.h"

/**
 * \class OnePointFormationView
 * \brief View model of the 1PointFormation algorithm
 *
 * Assigning this class to a Robot corresponds to the 1 Point Formation view model, i.e. every Robot can see every other Robots
 * position, velocity and acceleration only in a limited radius. The coordinate-system and id of each Robot is not visible.
 *
 * @see https://wiki.math.uni-paderborn.de/pg-schwarm/StartSeite/AK/Szenarien
 *
 */

class OnePointFormationView: public virtual VelocityView,
		public virtual SphericView,
		public virtual AccelerationView,
		public virtual PositionView {
public:
	OnePointFormationView(double view_radius);
	virtual ~OnePointFormationView();
};

#endif /* ONE_POINT_FORMATION_VIEW_H_ */
